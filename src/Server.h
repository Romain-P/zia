//
// Created by romain on 2/27/19.
//

#ifndef ZIA_SERVER_H
#define ZIA_SERVER_H

#include "ServerConfig.h"


class Server {
public:
    static Server &instance();

    ServerConfig const &config() const;
    bool reloadConfig();

private:
    Server() = default;

private:
    ServerConfig _config;
};

#define server Server::instance()

#endif //ZIA_SERVER_H
