#include <cassert>
#include <chrono>
#include <sstream>
#include <string_view>
#include <variant>
#include <iostream>
#include "../Json_parser_src/json.h"
#include <gtest/gtest.h>

#pragma once

using namespace std::literals;
using namespace json;

//Just test

namespace JsonTest {
    inline json::Document LoadJSON(const std::string &s) {
        std::istringstream strm(s);
        return json::Load(strm);
    }

    inline std::string Print(const Node &node) {
        std::ostringstream out;
        Print(Document{node}, out);
        return out.str();
    }

    void MustFailToLoad(const std::string &s);

    template<typename Fn>
    void MustThrowLogicError(Fn fn) {
        try {
            fn();
            std::cerr << "logic_error is expected"sv << std::endl;
            assert(false);
        } catch (const std::logic_error &) {
        } catch (const std::exception &e) {
            std::cerr << "exception thrown: "sv << e.what() << std::endl;
            assert(false);
        } catch (...) {
            std::cerr << "Unexpected error"sv << std::endl;
            assert(false);
        }
    }
}
