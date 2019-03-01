//
// Created by romain on 2/28/19.
//

#ifndef ZIA_SESSION_H
#define ZIA_SESSION_H

#include "zia.h"
#include <boost/enable_shared_from_this.hpp>

class Session : public boost::enable_shared_from_this<Session> {
public:
    Session(sizet id, boost_io &io) : _id(id), _socket(io) {};

    void start();
    void send(std::vector<char> const &data);
    tcp::socket &socket();
    sizet id() const;

private:
    sizet _id;
    tcp::socket _socket;
};


#endif //ZIA_SESSION_H
