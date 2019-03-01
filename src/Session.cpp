//
// Created by romain on 2/28/19.
//

#include "Session.h"
#include <boost/bind.hpp>

void Session::asyncAwaitPacket() {
    asyncRead();
}

void Session::send(uint8_t *bytes, sizet length) {
    lock_t lock(_writeMutex);
    _writePending.emplace_back(bytes, bytes + length);

    if (_writePending.size() == 1)
        asyncWrite();
}

void Session::asyncRead() {
    auto handler = boost::bind(&Session::onRead, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred);

    _socket.async_read_some(boost::asio::buffer(_readBuffer), handler);
}

void Session::onRead(const error_code &error, sizet bytes) {
    if (error || !_socket.is_open()) {
        errors("read error encountered on session %zu", _id);
        return;
    }
    //TODO: on read
}

void Session::asyncWrite() {
    lock_t lock(_writeMutex);
    auto &next = _writePending.front();

    auto handler = boost::bind(&Session::onWritten, shared_from_this(), boost::asio::placeholders::error);
    boost::asio::async_write(_socket, boost::asio::buffer(next, next.size()), handler);
}

void Session::onWritten(error_code const &error) {
    _writeMutex.lock();

    sizet pendingPackets = _writePending.size();
    if (error || pendingPackets == 0)
        errors("write error encountered on session %zu", _id);
    else {
        //auto &sent = _writePending.front(); //might be useful for logs
        info("session %zu sent a http response", _id);
    }

    if (pendingPackets != 0)
        _writePending.pop_front();
    _writeMutex.unlock();

    if (pendingPackets > 1 && _socket.is_open())
        asyncWrite();
}

tcp::socket &Session::socket() {
    return _socket;
}

sizet Session::id() const {
    return _id;
}
