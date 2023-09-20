#include "json.h"

using namespace std;

namespace json {
    namespace {


        Node LoadNode(istream &input);

        Array LoadArray(istream &input) {
            Array result;
            char c;
            while (input >> c, c != ']') {
                if (c != ',') {
                    input.putback(c);
                    result.push_back(LoadNode(input));
                }
            }
            if (c != ']')
                throw ParsingError("Incorrect array's data");
            return result;
        }


        Number LoadNumber(std::istream &input) {
            using namespace std::literals;

            std::string parsed_num;

            auto read_char = [&parsed_num, &input] {
                parsed_num += static_cast<char>(input.get());
                if (!input) {
                    throw ParsingError("Failed to read number from stream"s);
                }
            };

            auto read_digits = [&input, read_char] {
                if (!std::isdigit(input.peek())) {
                    throw ParsingError("A digit is expected"s);
                }
                while (std::isdigit(input.peek())) {
                    read_char();
                }
            };

            if (input.peek() == '-') {
                read_char();
            }
            if (input.peek() == '0') {
                read_char();
            } else {
                read_digits();
            }

            bool is_int = true;
            if (input.peek() == '.') {
                read_char();
                read_digits();
                is_int = false;
            }

            if (int ch = input.peek(); ch == 'e' || ch == 'E') {
                read_char();
                if (ch = input.peek(); ch == '+' || ch == '-') {
                    read_char();
                }
                read_digits();
                is_int = false;
            }

            try {
                if (is_int) {
                    try {
                        return std::stoi(parsed_num);
                    } catch (...) {

                    }
                }
                return std::stod(parsed_num);
            } catch (...) {
                throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
            }
        }


        std::string LoadString(std::istream &input) {
            using namespace std::literals;

            auto it = std::istreambuf_iterator<char>(input);
            auto end = std::istreambuf_iterator<char>();
            std::string s;
            while (true) {
                if (it == end) {
                    throw ParsingError("String parsing error");
                }
                const char ch = *it;
                if (ch == '"') {
                    ++it;
                    break;
                } else if (ch == '\\') {
                    ++it;
                    if (it == end) {
                        throw ParsingError("String parsing error");
                    }
                    const char escaped_char = *(it);
                    switch (escaped_char) {
                        case 'n':
                            s.push_back('\n');
                            break;
                        case 't':
                            s.push_back('\t');
                            break;
                        case 'r':
                            s.push_back('\r');
                            break;
                        case '"':
                            s.push_back('"');
                            break;
                        case '\\':
                            s.push_back('\\');
                            break;
                        default:
                            throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
                    }
                } else if (ch == '\n' || ch == '\r') {
                    throw ParsingError("Unexpected end of line"s);
                } else {
                    s.push_back(ch);
                }
                ++it;
            }

            return s;
        }

        Node LoadBool(istream &input) {
            char c;
            std::string res;
            for (size_t i = 0; i < 4; i++) {
                if (input >> c)
                    res += c;
                else
                    throw ParsingError("Incorrect bool value");
            }
            if (res == "true" && !isalpha(input.peek()) && !isdigit(input.peek()))
                return Node{true};
            else if (input >> c; res + c == "false" && !isalpha(input.peek()) && !isdigit(input.peek()))
                return Node{false};
            else
                throw ParsingError("Incorrect bool value");
        }

        Dict LoadDict(istream &input) {
            if (input.peek() != '{')
                throw ParsingError("Diction parsing error");
            Dict result;
            char c;
            std::string key;
            while (input >> c) {
                if (c == '"')
                    getline(input, key, '"');
                else if (c == ':') {
                    result[key] = LoadNode(input);
                } else if (c == '}')
                    break;
            }
            if (c != '}')
                throw ParsingError("Diction parsing error");
            return result;
        }

        nullptr_t LoadNull(istream &input) {
            char c;
            std::string null;
            for (size_t i = 0; i < 4; i++) {
                if (input >> c) {
                    null += c;
                }
            }
            if (null == "null")
                return nullptr;
            throw ParsingError("Incorrect nullptr");
        }

        Node LoadNode(istream &input) {
            char c;
            input >> c;
            if (c == 'n' && input.peek() == 'u') {
                input.putback(c);
                return Node{LoadNull(input)};
            } else if (c == '[') {
                return LoadArray(input);
            } else if (c == '{') {
                input.putback(c);
                return Node{LoadDict(input)};
            } else if (c == '"')
                return Node{LoadString(input)};
            else if (c == 't' || c == 'f') {
                input.putback(c);
                return Node{LoadBool(input)};
            } else {
                input.putback(c);
                return Node{LoadNumber(input)};
            }
        }

    }

    bool Node::operator==(const json::Node &another_node) const {
        return another_node.GetValue().index() == this->GetValue().index() &&
               another_node.GetValue() == this->GetValue();
    }

    bool Node::operator!=(const json::Node &another_node) const {
        return !(*this == another_node);
    }


    Node::Node() : value_(nullptr) {}

    Node::Node(bool bl) : value_(bl) {}

    Node::Node(nullptr_t null) : value_(null) {}

    Node::Node(Number number) {
        if (std::holds_alternative<int>(number))
            value_ = get<int>(number);
        else
            value_ = get<double>(number);
    }


    Node::Node(int number) : value_(number) {}

    Node::Node(double number) : value_(number) {}

    Node::Node(Dict map) : value_(std::move(map)) {}

    Node::Node(Array array) : value_(std::move(array)) {}

    Node::Node(std::string str) : value_(std::move(str)) {}

    Node::Node(Value value) : value_(std::move(value)) {}


    const Array &Node::AsArray() const {
        if (IsArray()) {
            return get<Array>(value_);
        }
        throw std::logic_error("");
    }

    const Dict &Node::AsMap() const {
        if (IsDict())
            return get<Dict>(value_);
        throw std::logic_error("");
    }

    int Node::AsInt() const {
        if (IsInt())
            return get<int>(value_);
        throw std::logic_error("");
    }

    const string &Node::AsString() const {
        if (IsString())
            return get<std::string>(value_);
        throw std::logic_error("");
    }

    double Node::AsDouble() const {
        if (IsPureDouble())
            return get<double>(value_);
        else if (IsInt())
            return get<int>(value_);
        throw std::logic_error("");
    }

    bool Node::AsBool() const {
        if (IsBool())
            return get<bool>(value_);
        throw std::logic_error("");
    }


    Document::Document(Node root)
            : root_(std::move(root)) {
    }

    const Node &Document::GetRoot() const {
        return root_;
    }

    Document Load(istream &input) {
        return Document{LoadNode(input)};
    }


    struct PrintContext {
        std::ostream &out;
        int indent_step = 4;
        int indent = 0;

        void PrintIndent() const {
            for (int i = 0; i < indent; ++i) {
                out.put(' ');
            }
        }


        // Возвращает новый контекст вывода с увеличенным смещением
        PrintContext Indented() const {
            return {out, indent_step, indent_step + indent};
        }
    };


    template<typename Value>
    void PrintValue(const Value &value, const PrintContext &ctx) {
        if (value.IsDouble()) {
            ctx.out << value.AsDouble();
        } else if (value.IsString()) {
            ctx.out << "\t\"" << value.AsString() << '\"';
        } else if (value.IsBool()) {
            if (value.AsBool())
                ctx.out << "\ttrue";
            else
                ctx.out << "\tfalse";
        } else if (value.IsArray()) {
            ctx.out << "[";
            for (auto &val: value.AsArray()) {
                PrintValue(val, ctx);
                ctx.out ;
            }
            ctx.out << "]";
        } else if (value.IsDict()) {
            ctx.out << "\n {\n";
            for (const auto &[key, val]: value.AsMap()) {
                ctx.out << "\t\"" << key << "\": ";
                PrintValue(val, ctx);
                ctx.out << '\n';
            }
            ctx.out << " }\n";
        } else
            ctx.out << nullptr;
    }

    void Print(const Document &doc, std::ostream &output) {
        PrintValue(doc.GetRoot(), PrintContext{output});
    }


}  // namespace json