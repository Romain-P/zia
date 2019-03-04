//
// Created by romain on 3/1/19.
//

#include "Network.h"
#include "Server.h"

void Network::start() {
    info("tcp server listening on %s:%d", server.config().address().c_str(), server.config().port());

    _thread = boost::thread([this]() {
        boost::asio::socket_base::reuse_address option(true);
        _io = std::make_unique<boost_io>();
        _worker = std::make_unique<boost::asio::thread_pool>(server.config().poolSize());

        try {
            _acceptor = std::make_unique<tcp::acceptor>(*(_io.get()), tcp::endpoint(tcp::v4(), server.config().port()));
            _acceptor->set_option(option);

            asyncAccept();

            _io->run();
        } catch (std::exception &e) {
            errors("tcp server error: %s", e.what());
        }
        info("tcp server stopped");
        _thread.detach();
    });
}

void Network::restart() {
    stop();
    if (_thread.joinable())
        _thread.join();
    start();
}

void Network::stop() {
    lock_t lock(_locker);

    _io->stop();
    for (auto &it: _sessions) {
        tcp::socket &socket = it.second->socket();

        if (socket.is_open())
            socket.close();
    }
    _acceptor->close();
    _worker->stop();
    _sessions.clear();
    _sessionCounter = 0;
    info("tcp server cleared successfully");
}

void Network::addSession(ptr<Session> session) {
    server.submit([this, session]() {
        lock_t lock(_locker);
        _sessions[session->id()] = session;
    });
}

void Network::delSession(ptr<Session> session, bool async) {
    if (async) {
        server.submit([this, session]() {
            delSession(session, false);
        });
        return;
    }

    lock_t lock(_locker);
    auto found = _sessions.find(session->id());

    if (found != _sessions.end()) {
        if (session->socket().is_open())
            session->socket().close();
        _sessions.erase(session->id());
    }
}

void Network::asyncAccept() {
    lock_t lock(_locker);

    auto session = boost::make_shared<Session>(++_sessionCounter, *(_io.get()));
    auto handler = boost::bind(&Network::onAccept, this, session, boost::asio::placeholders::error);

    _acceptor->async_accept(session->socket(), handler);
    _sessions[session->id()] = session;
}

void Network::onAccept(ptr<Session> session, const error_code &error) {
    if (_io->stopped()) return;

    if (!error) {
        server.submit([this, session]() {
            try {
                session->start();
            } catch (std::exception &e) {
                errors("session %zu stopped: %s", session->id(), e.what());
            }
            delSession(session, false);
        });
    }
    asyncAccept();
}

boost::thread &Network::thread() {
    return _thread;
}

boost::asio::thread_pool &Network::worker() {
    return *_worker.get();
}
