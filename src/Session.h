//
// Created by romain on 2/28/19.
//

#ifndef ZIA_SESSION_H
#define ZIA_SESSION_H

#include "zia.h"
#include <deque>
#include <boost/enable_shared_from_this.hpp>

class Session : public boost::enable_shared_from_this<Session> {
public:
    Session(sizet id, boost_io &io) : _id(id), _socket(io), _readBuffer() {};

    void asyncAwaitPacket();
    void send(uint8_t *bytes, sizet length);
    tcp::socket &socket();
    sizet id() const;
private:

    void asyncRead();
    void onRead(const error_code &error, sizet bytes);

    void asyncWrite();
    void onWritten(error_code const &error);

private:
    sizet _id;
    tcp::socket _socket;
    std::array<char, 10240> _readBuffer;
    std::deque<std::vector<char>> _writePending;
    std::mutex _writeMutex;
};


#endif //ZIA_SESSION_H
