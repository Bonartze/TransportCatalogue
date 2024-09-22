#include <cassert>
#include <chrono>
#include <sstream>
#include <string_view>
#include <variant>
#include <iostream>
#include "../Json_parser_src/json.h"
#include "JsonTests.hpp"

using namespace std::literals;
using namespace json;

//Just tests

namespace JsonTest {
    void MustFailToLoad(const std::string &s) {
        try {
            LoadJSON(s);
            std::cerr << "ParsingError exception is expected on '"sv << s << "'"sv << std::endl;
            assert(false);
        } catch (const json::ParsingError &) {
        } catch (const std::exception &e) {
            std::cerr << "exception thrown: "sv << e.what() << std::endl;
            assert(false);
        } catch (...) {
            std::cerr << "Unexpected error"sv << std::endl;
            assert(false);
        }
    }

    TEST(JSON_TEST, TEST_NULL) {
        std::cerr << "Test NULL OK\n";
        Node null_node;
        ASSERT_TRUE(null_node.IsNull());
        ASSERT_TRUE(!null_node.IsInt());
        ASSERT_TRUE(!null_node.IsDouble());
        ASSERT_TRUE(!null_node.IsPureDouble());
        ASSERT_TRUE(!null_node.IsString());
        ASSERT_TRUE(!null_node.IsArray());
        ASSERT_TRUE(!null_node.IsMap());

        Node null_node1{nullptr};
        ASSERT_TRUE(null_node1.IsNull());

        ASSERT_TRUE(null_node == null_node1);
        ASSERT_TRUE(!(null_node != null_node1));

        const Node node = LoadJSON("null"s).GetRoot();
        ASSERT_TRUE(node.IsNull());
        ASSERT_TRUE(node == null_node);
        ASSERT_TRUE(LoadJSON(" \t\r\n\n\r null \t\r\n\n\r "s).GetRoot() == null_node);
    }


    TEST(JSON_TEST, TEST_NUMBERS) {
        std::cerr << "Test Numbers OK\n";
        const Node int_node{42};
        ASSERT_TRUE(int_node.IsInt());
        ASSERT_TRUE(int_node.AsInt() == 42);
        ASSERT_TRUE(int_node.IsDouble());
        ASSERT_TRUE(int_node.AsDouble() == 42.0);
        ASSERT_TRUE(!int_node.IsPureDouble());
        ASSERT_TRUE(int_node == Node{42});
        ASSERT_TRUE(int_node != Node{42.0});

        const Node dbl_node{123.45};
        ASSERT_TRUE(dbl_node.IsDouble());
        ASSERT_TRUE(dbl_node.AsDouble() == 123.45);
        ASSERT_TRUE(dbl_node.IsPureDouble());
        ASSERT_TRUE(!dbl_node.IsInt());

        ASSERT_TRUE(Print(int_node) == "42"s);
        ASSERT_TRUE(Print(dbl_node) == "123.45"s);
        ASSERT_TRUE(Print(Node{-42}) == "-42"s);
        ASSERT_TRUE(Print(Node{-3.5}) == "-3.5"s);

        ASSERT_TRUE(LoadJSON("42"s).GetRoot() == int_node);
        ASSERT_TRUE(LoadJSON("123.45"s).GetRoot() == dbl_node);
        ASSERT_TRUE(LoadJSON("0.25"s).GetRoot().AsDouble() == 0.25);
        ASSERT_TRUE(LoadJSON("3e5"s).GetRoot().AsDouble() == 3e5);
        ASSERT_TRUE(LoadJSON("1.2e-5"s).GetRoot().AsDouble() == 1.2e-5);
        ASSERT_TRUE(LoadJSON("1.2e+5"s).GetRoot().AsDouble() == 1.2e5);
        ASSERT_TRUE(LoadJSON("-123456"s).GetRoot().AsInt() == -123456);
        ASSERT_TRUE(LoadJSON("0").GetRoot() == Node{0});
        ASSERT_TRUE(LoadJSON("0.0").GetRoot() == Node{0.0});
        ASSERT_TRUE(LoadJSON(" \t\r\n\n\r 0.0 \t\r\n\n\r ").GetRoot() == Node{0.0});
    }

    TEST(JSON_TEST, TEST_STRINGS) {
        std::cerr << "Test Strings OK\n";
        Node str_node{"Hello, \"everybody\""s};
        ASSERT_TRUE(str_node.IsString());
        ASSERT_TRUE(str_node.AsString() == "Hello, \"everybody\""s);
        ASSERT_TRUE(!str_node.IsInt());
        ASSERT_TRUE(!str_node.IsDouble());
        ASSERT_TRUE(LoadJSON("\t\r\n\n\r \"Hello\" \t\r\n\n\r ").GetRoot() == Node{"Hello"s});
    }

    TEST(JSON_TEST, TEST_BOOL) {
        std::cerr << "Test Bool OK\n";
        Node true_node{true};
        ASSERT_TRUE(true_node.IsBool());
        ASSERT_TRUE(true_node.AsBool());

        Node false_node{false};
        ASSERT_TRUE(false_node.IsBool());
        ASSERT_TRUE(!false_node.AsBool());

        ASSERT_TRUE(Print(true_node) == "true"s);
        ASSERT_TRUE(Print(false_node) == "false"s);

        ASSERT_TRUE(LoadJSON("true"s).GetRoot() == true_node);
        ASSERT_TRUE(LoadJSON("false"s).GetRoot() == false_node);
        ASSERT_TRUE(LoadJSON(" \t\r\n\n\r true \r\n"s).GetRoot() == true_node);
        ASSERT_TRUE(LoadJSON(" \t\r\n\n\r false \t\r\n\n\r "s).GetRoot() == false_node);
    }

    TEST(JSON_TEST, TEST_ARRAY) {
        std::cerr << "Test Array OK\n";
        Node arr_node{Array{1, 1.23, "Hello"s}};
        ASSERT_TRUE(arr_node.IsArray());
        const Array &arr = arr_node.AsArray();
        ASSERT_TRUE(arr.size() == 3);
        ASSERT_TRUE(arr.at(0).AsInt() == 1);


        ASSERT_TRUE(LoadJSON("[1,1.23,\"Hello\"]"s).GetRoot() == arr_node);
        ASSERT_TRUE(LoadJSON(Print(arr_node)).GetRoot() == arr_node);
        ASSERT_TRUE(LoadJSON(R"(  [ 1  ,  1.23,  "Hello"   ]   )"s).GetRoot() == arr_node);
        ASSERT_TRUE(LoadJSON("[ 1 \r \n ,  \r\n\t 1.23, \n \n  \t\t  \"Hello\" \t \n  ] \n  "s).GetRoot()
            == arr_node);
    }

    TEST(JSON_TEST, TEST_MAP) {
        std::cerr << "Test Map OK\n";
        Node dict_node{
            Dict{
                {"key1"s, "value1"s},
                {"key2"s, 42}
            }
        };
        ASSERT_TRUE(dict_node.IsMap());
        const Dict &dict = dict_node.AsMap();
        ASSERT_TRUE(dict.size() == 2);
        ASSERT_TRUE(dict.at("key1"s).AsString() == "value1"s);
        ASSERT_TRUE(dict.at("key2"s).AsInt() == 42);
        ASSERT_TRUE(LoadJSON("{ \"key1\": \"value1\", \"key2\": 42 }"s).GetRoot() == dict_node);
        ASSERT_TRUE(LoadJSON(Print(dict_node)).GetRoot() == dict_node);
        ASSERT_TRUE(
            LoadJSON(
                "\t\r\n\n\r { \t\r\n\n\r \"key1\" \t\r\n\n\r: \t\r\n\n\r \"value1\" \t\r\n\n\r , \t\r\n\n\r \"key2\" \t\r\n\n\r : \t\r\n\n\r 42 \t\r\n\n\r } \t\r\n\n\r"s
            )
            .GetRoot()
            == dict_node);
    }

    TEST(JSON_TEST, TEST_ERROR_HANDLING) {
        Node dbl_node{3.5}, array_node{Array{}};
        std::cerr << "Test Error Handling OK\n";
        EXPECT_THROW(LoadJSON("["s), json::ParsingError);
        EXPECT_THROW(LoadJSON("["s), json::ParsingError);

        EXPECT_THROW(LoadJSON("{"s), json::ParsingError);
        EXPECT_THROW(LoadJSON("}"s), json::ParsingError);

        EXPECT_THROW(LoadJSON("\"hello"s), json::ParsingError);

        EXPECT_THROW(LoadJSON("tru"s), json::ParsingError);
        EXPECT_THROW(LoadJSON("fals"s), json::ParsingError);
        EXPECT_THROW(LoadJSON("nul"s), json::ParsingError);

        EXPECT_THROW(dbl_node.AsInt(), std::logic_error);
        EXPECT_THROW(dbl_node.AsString(), std::logic_error);
        EXPECT_THROW(dbl_node.AsArray(), std::logic_error);

        EXPECT_THROW(array_node.AsMap(), std::logic_error);
        EXPECT_THROW(array_node.AsDouble(), std::logic_error);
        EXPECT_THROW(array_node.AsBool(), std::logic_error);
    }

    TEST(JSON_TEST, TEST_BENCHMARK) {
        const auto start = std::chrono::steady_clock::now();
        Array arr;
        arr.reserve(1'000);
        for (int i = 0; i < 1'000; ++i) {
            arr.emplace_back(Dict{
                {"int"s, 42},
                {"double"s, 42.1},
                {"null"s, nullptr},
                {"string"s, "hello"s},
                {"array"s, Array{1, 2, 3}},
                {"bool"s, true},
                {"map"s, Dict{{"key"s, "value"s}}},
            });
        }
        std::stringstream strm;
        json::Print(Document{arr}, strm);
        const auto doc = json::Load(strm);
        ASSERT_EQ(doc.GetRoot(), arr);
        const auto duration = std::chrono::steady_clock::now() - start;
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms"sv
                << std::endl;
    }
}
