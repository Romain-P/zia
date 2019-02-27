//
// Created by romain on 2/27/19.
//

#include "ServerConfig.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

ServerConfig::ServerConfig(std::string const &configPath) : _configPath(configPath) {}

bool ServerConfig::load() {
    try {
        loadProperties();
    } catch (std::exception &e) {
        return false;
    }
    return true;
}

void ServerConfig::loadProperties() {
    std::ifstream stream(_configPath);
    json data;

    stream >> data;

    if (data.find("address") != data.end())
        _address = data["address"];

    if (data.find("port") != data.end())
        _port = data["port"];

    if (data.find("modulesPath") != data.end())
        _modulesPath = data["modulesPath"];
}

uint16_t ServerConfig::port() const {
    return _port;
}

std::string const &ServerConfig::address() const {
    return _address;
}

std::string const &ServerConfig::modulesPath() const {
    return _modulesPath;
}

ServerConfig::os ServerConfig::platform() const {
#ifdef _WIN32
   return WINDOWS;
#elif __linux__
    return LINUX;
#elif __unix__
    // Unix
#elif defined(_POSIX_VERSION)
    return LINUX;
#else
#   return NOT_SUPPORTED;
#endif
}

std::string ServerConfig::platform_str() const {
#ifdef _WIN32
    return "windows";
#elif __linux__
    return "linux";
#elif __unix__
    // Unix
#elif defined(_POSIX_VERSION)
    return "linux";
#else
#   return "not_supported";
#endif
}
