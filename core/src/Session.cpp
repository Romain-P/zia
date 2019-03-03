//
// Created by romain on 2/28/19.
//

#include "Session.h"
#include <boost/bind.hpp>
#include "Server.h"
#include "Http.h"

void Session::start() {
    _handlers = server.modules().moduleHandlersFactory();
    connectionStart();
    connectionRead();

    _writeBuffer = _readBuffer;
    connectionWrite();
    end();
}

void Session::connectionStart() {
    auto result = executePipeline([this](RequestHandler::pointer handler) {
        return handler->onConnectionStart(server.connectionInfos(), _socket);
    });

    assertTrue(result != http::code::internal_error, "error encountered when connection started");
}

void Session::connectionRead() {
    _readBuffer.resize(server.config().networkReadBuffer());

    auto result = executePipeline([this](RequestHandler::pointer handler) {
        return handler->onConnectionRead(server.connectionInfos(), _socket, _readBuffer, _readSize);
    });

    if (!_readSize)
        result = http::code::internal_error;

    assertTrue(result != http::code::internal_error, "no suitable module found for read sockets of incoming connections");
}

void Session::readRequest() {

}

void Session::connectionWrite() {
    sizet unused = _writeBuffer.size();

    auto result = executePipeline([this, &unused](RequestHandler::pointer handler) {
        return handler->onConnectionWrite(server.connectionInfos(), _socket, _writeBuffer, unused);
    });

    assertTrue(result != http::code::internal_error, "no suitable module found for write on sockets");
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

tcp::socket &Session::socket() {
    return _socket;
}

sizet Session::id() const {
    return _id;
}
