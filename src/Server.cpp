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
    updateSharedConfig();

    if (!reloaded) {
        warning("missing or corrupted configuration file, default values used.");
    }

    //TODO: hook onConfigChange

    info("server properties\tserverName: %s, serverVersion: %s, address: %s, port: %d, modulesPath: %s",
            _config.serverName().c_str(), _config.serverVersion().c_str(), _config.address().c_str(), _config.port(), _config.modulesPath().c_str());

    std::string properties;
    bool first = true;
    for (auto &it: _config.modulesProperties()) {
        if (!first)
            properties += ", ";
        properties += it.first + ": " + it.second;
        if (first) first = false;
    }

    info("modules properties\t" + properties);
    return reloaded;
}

ZiaConfig const &Server::config() const {
    return _config;
}

ServerConfig const &Server::sharedConfig() const {
    return _sharedConfig;
}

void Server::updateSharedConfig() {
    _sharedConfig.name = _config.serverName();
    _sharedConfig.version = _config.serverVersion();
    _sharedConfig.platform = _config.platform();
    _sharedConfig.config = _config.modulesProperties();
    _connectionInfos.addr = _config.address();
    _connectionInfos.port = _config.port();
}

Connection const &Server::connectionInfos() const {
    return _connectionInfos;
}


