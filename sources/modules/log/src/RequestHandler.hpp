/*
** EPITECH PROJECT, 2019
** CPP_zia_2018
** File description:
** [log module] Request handler definition
*/

#pragma once

#include <iostream>
#include <chrono>
#include <boost/asio.hpp>
#include <Zia/API.hpp>

using boost::asio::ip::tcp;

namespace Zia {
namespace Modules {
namespace Log {

class RequestHandler : public API::RequestHandler
{
public:
    using clock_type = std::chrono::steady_clock;

    API::HookResultType onConnectionStart(const API::Connection& /* conn */, tcp::socket& /* sock */) override
    {
        _start = clock_type::now();
        return API::HookResult::Declined;
    }

    API::HookResultType onRequest(const API::Connection& /* conn */, const API::Request& req, API::Response& /* res */) override
    {
        _req = req;
        return API::HookResult::Declined;
    }

    API::HookResultType onResponse(const API::Connection& /* conn */, API::Response& res) override
    {
        _res = res;
        return API::HookResult::Declined;
    }

    API::HookResultType onConnectionEnd(const API::Connection& /* conn */, tcp::socket& /* sock */) override
    {
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(clock_type::now() - _start);
        std::cout
            << _req.method << " "
            << _req.uri << " "
            << std::to_string(_res.status_code) << " "
            << std::to_string(elapsed.count()) << "ms"
            << std::endl;
        return API::HookResult::Declined;
    }

private:
    clock_type::time_point _start;
    API::Request _req;
    API::Response _res;
};

}
}
}
