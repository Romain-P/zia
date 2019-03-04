//
// Created by romain on 3/1/19.
//

#include "Network.h"
#include "Server.h"

void Network::start() {
    info("tcp server listening on %s:%d", server.config().address().c_str(), server.config().port());

    _thread = boost::thread([this]() {
        info("1");
        boost::asio::socket_base::reuse_address option(true);
        info("2");
        _io = std::make_unique<boost_io>();
        info("3");
        _worker = std::make_unique<boost::asio::thread_pool>(server.config().poolSize());
        info("4");

        try {
            _acceptor = std::make_unique<tcp::acceptor>(*(_io.get()), tcp::endpoint(tcp::v4(), server.config().port()));
            info("5");
            _acceptor->set_option(option);
            info("6");

            asyncAccept();
            info("7");

            _io->run();
            info("8");
        } catch (std::exception &e) {
            info("9");
            errors("tcp server error: %s", e.what());
        }
        info("tcp server stopped");
        _thread.detach();
    });
    info("10");
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

    info("a");
    auto session = boost::make_shared<Session>(++_sessionCounter, *(_io.get()));
    info("b");
    auto handler = boost::bind(&Network::onAccept, this, session, boost::asio::placeholders::error);

    info("c");
    _acceptor->async_accept(session->socket(), handler);
    info("d");
    _sessions[session->id()] = session;
    info("e");
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
