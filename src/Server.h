//
// Created by romain on 2/27/19.
//

#ifndef ZIA_SERVER_H
#define ZIA_SERVER_H

#include "zia.h"
#include "ZiaConfig.h"

class Server {
public:
    static Server &instance();

    ZiaConfig const &config() const;
    ServerConfig const &sharedConfig() const;
    Connection const &connectionInfos() const;
    bool reloadConfig();
    void updateSharedConfig();

private:
    Server() = default;

private:
    ZiaConfig _config;
    ServerConfig _sharedConfig;
    Connection _connectionInfos;
};

#define server Server::instance()

#endif //ZIA_SERVER_H
