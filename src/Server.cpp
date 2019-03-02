//
// Created by romain on 2/27/19.
//

#include "Server.h"
#include <boost/bind.hpp>

Server &Server::instance() {
    static Server singleton;
    return singleton;
}

void Server::start() {
    reloadConfig();
    _modules.loadAll();
    _network.start();
}

bool Server::reloadConfig() {
    bool reloaded = _config.load();
    updateSharedConfig();

    if (!reloaded) {
        warning("missing or corrupted configuration file, default values used.");
    }

    //TODO: hook onConfigChange

    info("server properties\tserverName: %s, serverVersion: %s, address: %s, port: %d, poolSize: %d, modulesPath: %s",
            _config.serverName().c_str(), _config.serverVersion().c_str(), _config.address().c_str(),
            _config.port(), _config.poolSize(), _config.modulesPath().c_str());

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

void Server::updateSharedConfig() {
    _sharedConfig.name = _config.serverName();
    _sharedConfig.version = _config.serverVersion();
    _sharedConfig.platform = _config.platform();
    _sharedConfig.config = _config.modulesProperties();
    _connectionInfos.addr = _config.address();
    _connectionInfos.port = _config.port();
}

ZiaConfig const &Server::config() const {
    return _config;
}

ServerConfig const &Server::sharedConfig() const {
    return _sharedConfig;
}

Connection const &Server::connectionInfos() const {
    return _connectionInfos;
}
void Server::submit(std::function<void()> task) {
    boost::asio::post(_network.worker(), task);
}

Network &Server::network() {
    return _network;
}

Modules &Server::modules() {
    return _modules;
}

