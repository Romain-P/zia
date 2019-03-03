#pragma once

#include <boost/asio.hpp>
#include <string>
#include <stdio.h>
#include "Zia/API.hpp"
#include "zia.h"

using lock_t = std::lock_guard<std::mutex>;

//thread safe console outputs
namespace print_util {
    namespace {
        static std::mutex &locker() { //avoid copies by this hack
            static std::mutex mutex;
            return mutex;
        }

        template<typename...Args>
        inline void print_custom(std::string const &hint, std::string const &str, Args... args) {
            lock_t lock(locker());
            std::string output = "[" + hint + "]: " + str + "\n";
            fprintf(stderr, output.c_str(), args...);
        }

        template<typename...Args>
        inline void print_pure(std::string const &str, Args... args) {
            lock_t lock(locker());
            fprintf(stderr, str.c_str(), args...);
        }
    }

    template<typename...Args>
    inline void warning(std::string const &str, Args... args) {
        print_custom("warning", str, args...);
    }

    template<typename...Args>
    inline void errors(std::string const &str, Args... args) {
        print_custom("error", str, args...);
    }

    template<typename...Args>
    inline void info(std::string const &str, Args... args) {
        print_custom("info", str, args...);
    }

    template<typename...Args>
    inline void pureinfo(std::string const &str, Args... args) {
        print_pure(str, args...);
    }
}

namespace string_util {
    inline std::vector<std::string> split (const std::string &s, char delim) {
        std::vector<std::string> result;
        std::stringstream ss (s);
        std::string item;

        while (getline (ss, item, delim)) {
            result.push_back (item);
        }

        return result;
    }
}

namespace file_util {
    inline char separator()
    {
#ifdef _WIN32
        return '\\';
#else
        return '/';
#endif
    }
}

using namespace boost;
using namespace print_util;
using namespace string_util;
using namespace Zia::API;


template<class T>
using ptr = boost::shared_ptr<T>;
template<class T>
using uptr = std::unique_ptr<T>;
using sizet = boost::uintmax_t;
using ssizet = boost::intmax_t;
using session_id = sizet;
using error_code = boost::system::error_code;
using boost_io = boost::asio::io_service;
using boost::asio::ip::tcp;
using header_type = boost::uint64_t;