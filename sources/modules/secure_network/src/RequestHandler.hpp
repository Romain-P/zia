#include <memory>

#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <Zia/API.hpp>

using boost::asio::ip::tcp;
using namespace Zia::API;
using tls_socket = boost::asio::ssl::stream<tcp::socket&>;

class SecureNetworkRequestHandler : public RequestHandler {
public:
    SecureNetworkRequestHandler(std::shared_ptr<boost::asio::ssl::context> const &context) : RequestHandler(), _context(context) {}

    HookResultType onConnectionStart(const Connection &connection, tcp::socket &socket) override {
        _socket = std::make_unique<tls_socket>(socket, *_context);
        boost::system::error_code error;
        _socket->handshake(boost::asio::ssl::stream_base::server, error);

        if (error)
            throw std::runtime_error("TLS handshake invalidated, un-secure connections are not accepted");

        return HookResult::Ok;
    }

    HookResultType
    onConnectionRead(const Connection &connection, tcp::socket &socket, std::vector<char> &buffer,
                     size_t &size) override {
        boost::system::error_code error;
        size = _socket->read_some(boost::asio::buffer(buffer, buffer.size()), error);
        return HookResult::Ok;
    }

    HookResultType
    onConnectionWrite(const Connection &connection, tcp::socket &socket, const std::vector<char> &buffer,
                      size_t &size) override {
        boost::system::error_code error;
        size = boost::asio::write(*_socket, boost::asio::buffer(buffer), boost::asio::transfer_all(), error);
        return HookResult::Ok;
    }

private:
    std::shared_ptr<boost::asio::ssl::context> _context;
    std::unique_ptr<tls_socket> _socket;
};