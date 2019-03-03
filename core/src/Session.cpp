//
// Created by romain on 2/28/19.
//

#include "Session.h"
#include <boost/bind.hpp>
#include "Server.h"
#include "Http.h"

void Session::start() {
    _handlers = server.modules().moduleHandlersFactory();

    if (!connectionStart() || !connectionRead()) {
        errors("no suitable module found for read incoming connections");
        end();
        return;
    }

    pureinfo(&_readBuffer[0]);
    send(_readBuffer);
}

bool Session::connectionStart() {
    auto result = executePipeline([this](RequestHandler::pointer handler) {
        return handler->onConnectionStart(server.connectionInfos(), _socket);
    });

    return result != http::code::internal_error;
}

bool Session::connectionRead() {
    auto result = executePipeline([this](RequestHandler::pointer handler) {
        return handler->onConnectionRead(server.connectionInfos(), _socket, _readBuffer, _readSize);
    });

    if (!_readSize)
        result = http::code::internal_error;

    return result != http::code::internal_error;
}

HookResultType Session::executePipeline(std::function<HookResultType(RequestHandler::pointer)> hook) {
    return server.modules().executePipeline([this, hook](Module::pointer module) {
        auto it = _handlers.find(module->getName());

        if (it == _handlers.end())
            return HookResult::Declined; /* skip modules added during a request */

        RequestHandler::pointer handler = it->second;
        return hook(handler);
    });
}

void Session::end() {
    server.network().delSession(shared_from_this());
}

void Session::send(std::vector<char> const &data) {
    boost::system::error_code ignored_error;
    boost::asio::write(_socket, boost::asio::buffer(data),
                       boost::asio::transfer_all(), ignored_error);
}

tcp::socket &Session::socket() {
    return _socket;
}

sizet Session::id() const {
    return _id;
}
