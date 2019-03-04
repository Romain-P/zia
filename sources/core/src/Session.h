//
// Created by romain on 2/28/19.
//

#ifndef ZIA_SESSION_H
#define ZIA_SESSION_H

#include "zia.h"
#include <boost/enable_shared_from_this.hpp>
#include "Modules.h"
#include "Http.h"

class Session : public boost::enable_shared_from_this<Session> {
public:
    Session(sizet id, boost_io &io) : _id(id), _socket(io), _readBuffer() {};

    void start();
    tcp::socket &socket();
    sizet id() const;

private:
    void connectionStart();
    void connectionRead();
    void connectionWrite();
    void connectionEnd();

    void readRequest();
    void createResponse();
    void sendResponse();

    HookResultType executePipeline(std::function<HookResultType(RequestHandler::pointer)> hook);

private:
    Request _request;
    Response _response = http::responses::not_found;
    std::unordered_map<module_name, RequestHandler::pointer> _handlers;

private:
    sizet _id;
    tcp::socket _socket;
    std::vector<char> _readBuffer;
    std::vector<char> _writeBuffer;
};


#endif //ZIA_SESSION_H
