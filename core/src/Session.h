//
// Created by romain on 2/28/19.
//

#ifndef ZIA_SESSION_H
#define ZIA_SESSION_H

#include "zia.h"
#include <boost/enable_shared_from_this.hpp>
#include "Modules.h"

class Session : public boost::enable_shared_from_this<Session> {
public:
    Session(sizet id, boost_io &io) : _id(id), _socket(io), _readBuffer(), _readSize(0) {};

    void start();
    tcp::socket &socket();
    sizet id() const;

private: /** process **/
    void connectionStart();
    void connectionRead();
    void connectionWrite();

    void readRequest();
    void end();
    HookResultType executePipeline(std::function<HookResultType(RequestHandler::pointer)> hook);

private:
    Request _request;
    Response _response;
    std::unordered_map<module_name, RequestHandler::pointer> _handlers;

private:
    enum ReadState {
        READ_HEADERS,
        READ_BODY
    };

    sizet _id;
    tcp::socket _socket;
    std::vector<char> _readBuffer;
    sizet _readSize;
    std::vector<char> _writeBuffer;
};


#endif //ZIA_SESSION_H
