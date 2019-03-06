#pragma once

#include <iostream>
#include <chrono>
#include <array>
#include <boost/asio.hpp>
#include <Zia/API.hpp>

using boost::asio::ip::tcp;
using namespace Zia::API;

class PhpRequestHandler : public RequestHandler {
public:
    HookResultType
    onConnectionRead(const Connection &connection, tcp::socket &socket, std::vector<char> &buffer, size_t &size) override {
        boost::system::error_code error;
        size = socket.read_some(boost::asio::buffer(buffer, buffer.size()), error);

        return HookResult::Ok;
    }

    HookResultType
    onConnectionWrite(const Connection &connection, tcp::socket &socket, const std::vector<char> &buffer, size_t &size) override {
        boost::system::error_code error;
        socket.write_some(boost::asio::buffer(buffer), error);

        return HookResult::Ok;
    }
};