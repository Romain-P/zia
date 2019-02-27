#pragma once

#include <boost/asio.hpp>
#include <string>
#include <stdio.h>

namespace print_util {
    namespace {
        template<typename...Args>
        inline void print_custom(std::string const &hint, std::string const &str, Args... args) {
            std::string output = "[" + hint + "]: " + str + "\n";
            fprintf(stderr, output.c_str(), args...);
        }
    }
    template<typename...Args>
    inline void warning(std::string const &str, Args... args) {
        print_custom("warning", str, args...);
    }

    template<typename...Args>
    inline void error(std::string const &str, Args... args) {
        print_custom("error", str, args...);
    }

    template<typename...Args>
    inline void info(std::string const &str, Args... args) {
        print_custom("info", str, args...);
    }
}

using namespace boost;
using namespace print_util;