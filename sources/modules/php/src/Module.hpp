#pragma once

#include <iostream>
#include <string>
#include <Zia/API.hpp>

#include "RequestHandler.hpp"

class PhpModule : public Zia::API::Module
{
public:
    const std::string& getName() const override {
        return _name;
    }

    void onActivate(const ServerConfig &config) override {
        onConfigChange(config);
    }

    void onConfigChange(const ServerConfig &config) override {
        auto it = config.config.find("php_cgi_path");
        if (it == config.config.end())
            throw std::runtime_error("php_cgi_path undefined in config file");
        _phpBinaryPath = it->second;

        it = config.config.find("php_www_path");
        if (it == config.config.end())
            throw std::runtime_error("php_www_path undefined in config file");
        _phpWwwPath = it->second;
    }

    Zia::API::RequestHandler::pointer newRequestHandler() override {
        return std::make_shared<PhpRequestHandler>(_phpBinaryPath, _phpWwwPath);
    }

private:
    std::string _name = "php";
    std::string _phpBinaryPath;
    std::string _phpWwwPath;
};
