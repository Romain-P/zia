#pragma once

#include <iostream>
#include <chrono>
#include <array>
#include <Zia/API.hpp>
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <regex>

using namespace Zia::API;
using namespace boost::process;

class PhpRequestHandler : public RequestHandler {
public:
    PhpRequestHandler(std::string const &phpBinaryPath, std::string const &phpWwwPath)
            : RequestHandler(), _phpBinaryPath(phpBinaryPath), _phpWwwPath(phpWwwPath) {}

    HookResultType onRequest(const Connection &connection, const Request &request, Response &response) override {
        ipstream output_stream;
        opstream input_stream;
        child proc(_phpBinaryPath, std_out > output_stream, std_in < input_stream, buildCgiEnvironment(request));

        if (!request.body.empty())
            input_stream << &request.body[0];

        std::string output((std::istreambuf_iterator<char>(output_stream)), (std::istreambuf_iterator<char>()));
        parseCgi(output, request, response);
        proc.wait();
        return HookResult::Ok;
    }

    HookResultType onResponse(const Connection &connection, Response &response) override {
        static const std::string not_found = "The requested URL does not exist";
        switch (response.status_code) {
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
    static inline const auto ext_html_result = std::vector<std::string>{".html", ".htm", ".php", ".phtml", ".phtm"};
    static inline char const *beginPath =
#ifdef _WIN32
            ".\\";
#else
            "./";
#endif

    environment buildCgiEnvironment(Request const &request) {
        boost::process::environment cgi_env = boost::this_process::environment();

        cgi_env["SERVER_PROTOCOL"] = request.protocol;
        cgi_env["REQUEST_METHOD"] = request.method;
        cgi_env["GATEWAY_INTERFACE"] = "CGI/1.1";
        cgi_env["REDIRECT_STATUS"] = "200";
        cgi_env["DOCUMENT_ROOT"] = beginPath + _phpWwwPath;

        auto it = request.headers.find("Content-Length");
        if (it != request.headers.end())
            cgi_env["CONTENT_LENGTH"] = it->second;

        it = request.headers.find("Content-Type");
        if (it != request.headers.end())
            cgi_env["CONTENT_TYPE"] = it->second;

        setupUri(request.uri);

        auto queryIndex = _requestedUri.find('?');

        if (queryIndex != std::string::npos) {
            cgi_env["QUERY_STRING"] = _requestedUri.substr(queryIndex + 1, _requestedUri.size());
            _requestedUri = _requestedUri.substr(0, queryIndex);
        } else
            cgi_env["QUERY_STRING"] = _requestedUri;

        cgi_env["SCRIPT_FILENAME"] = beginPath + _phpWwwPath + _requestedUri;
        return cgi_env;
    }

    void parseCgi(std::string &data, Request const &request, Response &response) {
        static const std::regex regex_status("([0-9]+)\\s+(.+)");
        static const std::regex regex_header("(?:\n)?([^:\n]+):\\s?(.+)");
        static const std::string headers_end = "\r\n\r\n";

        auto body_index = data.find(headers_end);
        std::string headers = data.substr(0, body_index);
        std::string body = data.substr(body_index + headers_end.size(), data.size());

        std::smatch matcher;
        while (std::regex_search(headers, matcher, regex_header)) {
            response.headers[matcher[1]] = matcher[2];
            headers = matcher.suffix();
        }

        setResponseHeaders(response);

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

        response.body.insert(response.body.end(), body.begin(), body.end());
    }

    void setResponseHeaders(Response &response) {
        auto ext = boost::filesystem::extension(_requestedUri);
        if (ext.empty()) return;

        auto it = response.headers.find("Content-type");
        if (std::find(ext_html_result.begin(), ext_html_result.end(), ext) == ext_html_result.end())
            response.headers.erase(it);
        else {
            response.headers["Content-Type"] = it->second;
            response.headers.erase(it);
        }

        it = response.headers.find("Content-length");
        if (it != response.headers.end()) {
            response.headers["Content-Length"] = it->second;
            response.headers.erase(it);
        }
    }

    void setupUri(std::string const &uri) {
        _requestedUri = uri;

        if (uri != "/" && uri.rfind("/?", 0) != 0) return;

        for (auto &ext: ext_html_result) {
            if (boost::filesystem::exists(beginPath + _phpWwwPath + "/index" + ext)) {
                _requestedUri = "/index" + ext;

                if (uri.size() > 1)
                    _requestedUri += uri.substr(1);
                return;
            }
        }
    }

private:
    std::string _phpBinaryPath;
    std::string _phpWwwPath;
    std::string _requestedUri;
};