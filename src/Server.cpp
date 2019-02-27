//
// Created by romain on 2/27/19.
//

#include "Server.h"

Server &Server::instance() {
    static Server singleton;
    return singleton;
}

bool Server::reloadConfig() {
    bool reloaded = _config.load();

    if (!reloaded) {
        warning("missing or corrupted configuration file, default values used.");
    }

    info("properties address: %s, port: %d, modulesPath: %s",
            _config.address().c_str(), _config.port(), _config.modulesPath().c_str());
    return reloaded;
}

ServerConfig const &Server::config() const {
    return _config;
}
