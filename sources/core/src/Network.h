//
// Created by romain on 3/1/19.
//

#ifndef ZIA_NETWORK_H
#define ZIA_NETWORK_H

#include "zia.h"
#include "Session.h"
#include <unordered_map>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/thread.hpp>

class Network {
public:
    Network() : _io(), _acceptor(_io, tcp::v4()), _sessionCounter(0) {};

    void start();
    void restart();
    void stop();

    void addSession(ptr<Session> session);
    void delSession(ptr<Session> session, bool async = true);

    boost::thread &thread();
    boost::asio::thread_pool &worker();

private:
    boost_io _io;
    tcp::acceptor _acceptor;
    sizet _sessionCounter;
    std::unordered_map<session_id, ptr<Session>> _sessions;
    std::mutex _locker;
    boost::thread _thread;
    uptr<boost::asio::thread_pool> _worker;

    void asyncAccept();
    void onAccept(ptr<Session> session, const error_code &error);
};


#endif //ZIA_NETWORK_H
