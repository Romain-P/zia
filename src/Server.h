//
// Created by romain on 2/27/19.
//

#ifndef ZIA_SERVER_H
#define ZIA_SERVER_H

#include "zia.h"
#include "ZiaConfig.h"
#include "Session.h"
#include <boost/thread.hpp>
#include <unordered_map>

class Server {
public:
    static Server &instance();

    ZiaConfig const &config() const;
    ServerConfig const &sharedConfig() const;
    Connection const &connectionInfos() const;
    bool reloadConfig();
    void updateSharedConfig();

    void start();
    void restart();
    void stop();

    void startNetwork();
    boost::thread &thread();

private:
    Server() : _sessionCounter(0) {}

private: /** Network **/
    sizet _sessionCounter;
    uptr<boost_io> _io;
    uptr<tcp::acceptor> _acceptor;
    std::unordered_map<session_id, ptr<Session>> _sessions;
    boost::thread _thread;

    void asyncAccept();
    void onAccept(ptr<Session> session, const error_code &error);

private: /** Core **/
    ZiaConfig _config;
    ServerConfig _sharedConfig;
    Connection _connectionInfos;
};

#define server Server::instance()

#endif //ZIA_SERVER_H
