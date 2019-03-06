#pragma once

#include <iostream>
#include <string>
#include <Zia/API.hpp>
#include <fstream>

#include "RequestHandler.hpp"

class SecureNetworkModule : public Zia::API::Module {
public:

    void onConfigChange(const ServerConfig &config) override {
        createContextSSL(config);
    }

    const std::string &getName() const override {
        return _name;
    }

    Zia::API::RequestHandler::pointer newRequestHandler() override {
        return std::make_shared<SecureNetworkRequestHandler>(_sslContext);
    }

private:
    inline static const std::string certificate = "tls_certificate_path";
    inline static const std::string privateKey = "tls_private_key_path";

    void createContextSSL(const ServerConfig &config) {
        if (config.config.find(certificate) == config.config.end() ||
            config.config.find(privateKey) == config.config.end())
            throw std::runtime_error(std::string("missing config properties (<" + certificate + "> or/and <" + privateKey + ">)").c_str());

        auto certPath = config.config.at(certificate);
        auto keyPath = config.config.at(privateKey);

        auto cert = loadFile(certPath);
        auto key = loadFile(keyPath);

        _sslContext = std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12_server);
        _sslContext->set_options(
                boost::asio::ssl::context::tlsv12_server |
                boost::asio::ssl::context::no_tlsv1_1);

        _sslContext->use_certificate_chain(
                boost::asio::buffer(cert.data(), cert.size()));

        _sslContext->use_private_key(
                boost::asio::buffer(key.data(), key.size()),
                boost::asio::ssl::context::file_format::pem);
    }

    std::string loadFile(std::string const &path) const {
        std::ifstream ifs(path);

        if (!ifs.is_open())
            throw std::runtime_error(std::string("file <" + path + "> not found").c_str());

        return std::string((std::istreambuf_iterator<char>(ifs)),
                           (std::istreambuf_iterator<char>()));
    }

private:
    std::string _name = "secure-network";
    std::shared_ptr<boost::asio::ssl::context> _sslContext;
};
