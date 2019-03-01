//
// Created by romain on 2/28/19.
//

#include "Session.h"
#include <boost/bind.hpp>

void Session::start() {
    std::array<char, 8096> _buffer = {0};
    std::size_t _length;


}

void Session::send(std::vector<char> const &data) {
    boost::system::error_code ignored_error;
    boost::asio::write(_socket, boost::asio::buffer(data),
                       boost::asio::transfer_all(), ignored_error);
}

tcp::socket &Session::socket() {
    return _socket;
}

sizet Session::id() const {
    return _id;
}