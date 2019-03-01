//
// Created by romain on 2/27/19.
//

#include "Server.h"
#include <boost/bind.hpp>

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

void Server::start() {
    reloadConfig();
    info("tcp server listening on %s:%d", _config.address().c_str(), _config.port());
    _thread = boost::thread(boost::bind(&Server::startNetwork, this));
}

void Server::startNetwork() {
    boost::asio::socket_base::reuse_address option(true);
    _worker = std::make_unique<boost::asio::thread_pool>(_config.poolSize());
    _io = std::make_unique<boost_io>();

    try {
        _acceptor = std::make_unique<tcp::acceptor>(*(_io.get()), tcp::endpoint(tcp::v4(), _config.port()));
        _acceptor->set_option(option);

        asyncAccept();

        _io->run();
    } catch (std::exception &e) {
        errors("tcp server error: %s", e.what());
    }
    info("tcp server stopped");
    _thread.detach();
}

void Server::restart() {
    stop();
    if (_thread.joinable())
        _thread.join();
    start();
}

void Server::stop() {
    _worker->stop();
    _io->stop();
    for (auto &it: _sessions) {
        tcp::socket &socket = it.second->socket();

        if (socket.is_open())
            socket.close();
    }
    _acceptor->close();
    _sessions.clear();
    _sessionCounter = 0;
    info("tcp server cleared successfully");
}

void Server::asyncAccept() {
    auto session = boost::make_shared<Session>(++_sessionCounter, *(_io.get()));
    auto handler = boost::bind(&Server::onAccept, this, session, boost::asio::placeholders::error);

    _acceptor->async_accept(session->socket(), handler);
    _sessions[session->id()] = session;
}

void Server::onAccept(ptr<Session> session, const error_code &error) {
    if (_io->stopped()) return;

    if (!error) {
        //TODO: on connect
        session->asyncAwaitPacket();
    }
    asyncAccept();
}

boost::thread &Server::thread() {
    return _thread;
}

void Server::submit(std::function<void()> task) {
    boost::asio::post(*_worker.get(), task);
}


