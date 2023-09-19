#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <variant>

namespace json {

    class Node;

    using Number = std::variant<int, double>;
    using Dict = std::map<std::string, Node>;
    using Array = std::vector<Node>;

// Эта ошибка должна выбрасываться при ошибках парсинга JSON
    class ParsingError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    class Node {
        using Value = std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string>;
    public:

/*
        std::ostream &operator<<(std::ostream &out);
*/

        /* Реализуйте Node, используя std::variant */
        bool operator==(const Node &another_node) const;

        bool operator!=(const Node &another_node) const;

        Node();

        Node(bool);

        Node(Value);

        Node(Number);

        Node(int);

        Node(double);

        Node(Array);

        Node(std::string);

        Node(Dict);

        Node(nullptr_t);

        /* explicit Node(Array array);

         explicit Node(Dict map);

         explicit Node(int value);

         explicit Node(std::string value);*/

        [[nodiscard]]

        const Array &AsArray() const;

        [[nodiscard]] const Dict &AsMap() const;

        [[nodiscard]] int AsInt() const;

        [[nodiscard]] const std::string &AsString() const;

        [[nodiscard]] double AsDouble() const;

        [[nodiscard]] bool AsBool() const;

        [[nodiscard]] const Value &GetValue() const {
            return value_;
        }

        [[nodiscard]] bool IsInt() const {
            return std::holds_alternative<int>(value_);
        }

        [[nodiscard]] bool IsDouble() const {
            return std::holds_alternative<double>(value_) || IsInt();
        }

        [[nodiscard]] bool IsPureDouble() const {
            return std::holds_alternative<double>(value_);
        }

        [[nodiscard]] bool IsBool() const {
            return std::holds_alternative<bool>(value_);
        }

        [[nodiscard]] bool IsMap() const {
            return std::holds_alternative<Dict>(value_);
        }

        [[nodiscard]] bool IsString() const {
            return std::holds_alternative<std::string>(value_);
        }

        [[nodiscard]] bool IsNull() const {
            return std::holds_alternative<nullptr_t>(value_);
        }

        [[nodiscard]] bool IsArray() const {
            return std::holds_alternative<Array>(value_);
        }

        [[nodiscard]] bool IsDict() const {
            return std::holds_alternative<Dict>(value_);
        }


    private:
        Value value_;
    };

    class Document {
    public:
        explicit Document(Node root);

        const Node &GetRoot() const;

    private:
        Node root_;
    };

    Document Load(std::istream &input);

    void Print(const Document &doc, std::ostream &output);

}  // namespace json