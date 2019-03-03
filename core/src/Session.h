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
    Session(sizet id, boost_io &io) : _id(id), _socket(io) {};

    void start();
    tcp::socket &socket();
    sizet id() const;

private: /** process **/
    bool connectionStart();
    bool connectionRead();
    void end();

    Request _request;
    Response _response;
    HookResultType executePipeline(std::function<HookResultType(RequestHandler::pointer)> hook);

private:
    std::unordered_map<module_name, RequestHandler::pointer> _handlers;

private: /** network **/
    void send(std::vector<char> const &data);

private:
    sizet _id;
    tcp::socket _socket;
    std::vector<char> _readBuffer;
    sizet _readSize;
};


#endif //ZIA_SESSION_H
