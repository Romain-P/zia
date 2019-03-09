#pragma once

#include <iostream>
#include <chrono>
#include <array>
#include <boost/asio.hpp>
#include <Zia/API.hpp>
#include <boost/process.hpp>
#include <regex>

using namespace Zia::API;
using namespace boost::process;

class PhpRequestHandler : public RequestHandler {
public:
    PhpRequestHandler(std::string const &phpBinaryPath, std::string const &phpWwwPath)
        : RequestHandler(), _phpBinaryPath(phpBinaryPath), _phpWwwPath(phpWwwPath) {}

    HookResultType onRequest(const Connection &connection, const Request &request, Response &response) override {
        ipstream pipe_stream;
        child proc(_phpBinaryPath, std_out > pipe_stream, buildCgiEnvironment(request));

        std::string output((std::istreambuf_iterator<char>(pipe_stream)), (std::istreambuf_iterator<char>()));
        parseCgi(output, response);

        proc.wait();
        return HookResult::Ok;
    }

    HookResultType onResponse(const Connection &connection, Response &response) override {
        static const std::string not_found = "The requested URL does not exist";
        switch(response.status_code) {
            case 404:
                response.body.clear();
                response.body.insert(response.body.end(), not_found.begin(), not_found.end());
                break;
            default:
                break;
        }
        return HookResult::Ok;
    }

    HookResultType onRequestError(const Connection &connection, int error, Response &response) override {
        response.status_code = uint16_t(error);
        response.status_message = "Error";
        response.protocol = "HTTP/1.1";
        response.body.clear();
        return HookResult::Ok;
    }

private:
    environment buildCgiEnvironment(Request const &request) {
        boost::process::environment cgi_env = boost::this_process::environment();

        static char const *beginPath =
#ifdef _WIN32
            ".\\";
#else
            "./";
#endif

        cgi_env["SCRIPT_FILENAME"] = beginPath + _phpWwwPath + request.uri;
        cgi_env["SERVER_PROTOCOL"] = request.protocol;
        cgi_env["REQUEST_METHOD"] = request.method;
        cgi_env["GATEWAY_INTERFACE"] = "CGI/1.1";
        cgi_env["REDIRECT_STATUS"] = "200";

        return cgi_env;
    }

    void parseCgi(std::string &data, Response &response) {
        static const std::regex regex_status("([0-9]+)\\s+(.+)");
        static const std::regex regex_header("([^:\n]+):\\s?(.+)");
        static const std::string headers_end = "\r\n\r\n";

        std::smatch matcher;
        while (std::regex_search(data, matcher, regex_header)) {
            response.headers[matcher[1]] = matcher[2];
            data = matcher.suffix();
        }

        auto it = response.headers.find("Status");
        if (it != response.headers.end()) {
            auto status = it->second;

            std::regex_search(status, matcher, regex_status);
            response.status_code = uint16_t(std::stoi(matcher[1]));
            response.status_message = matcher[2];
        } else {
            response.status_code = 200;
            response.status_message = "OK";
        }

        std::string body = data.substr(headers_end.size());
        response.body.insert(response.body.end(), body.begin(), body.end());
    }

private:
    std::string _phpBinaryPath;
    std::string _phpWwwPath;
};