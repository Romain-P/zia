//
// Created by romain on 2/27/19.
//

#ifndef ZIA_SERVER_H
#define ZIA_SERVER_H

#include "zia.h"
#include "ZiaConfig.h"
#include "Session.h"
#include "Network.h"
#include "Modules.h"
#include <boost/thread.hpp>

class Server {
public:
    static Server &instance();

    void start();
    bool reloadConfig();
    void updateSharedConfig();
    void submit(std::function<void()> task);

    ZiaConfig const &config() const;
    ServerConfig const &sharedConfig() const;
    Connection const &connectionInfos() const;
    Network &network();
    Modules &modules();

private:
    Server() = default;

private:
    ZiaConfig _config;
    ServerConfig _sharedConfig;
    Connection _connectionInfos;
    Network _network;
    Modules _modules;
};

#define server Server::instance()

#endif //ZIA_SERVER_H
