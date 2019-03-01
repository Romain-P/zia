//
// Created by romain on 2/27/19.
//

#include "ZiaConfig.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

ZiaConfig::ZiaConfig(std::string const &configPath) : _configPath(configPath) {}

bool ZiaConfig::load() {
    try {
        loadProperties();
    } catch (std::exception &e) {
        return false;
    }
    return true;
}

void ZiaConfig::loadProperties() {
    std::ifstream stream(_configPath);
    json data;

    stream >> data;

    if (data.find("serverName") != data.end())
        _serverName = data["serverName"];

    if (data.find("serverVersion") != data.end())
        _serverVersion = data["serverVersion"];

    if (data.find("address") != data.end())
        _address = data["address"];

    if (data.find("port") != data.end())
        _port = data["port"];

    if (data.find("modulesPath") != data.end())
        _modulesPath = data["modulesPath"];

    //TODO: modulesProperties nested objects / make it recursive
    json modulesProperties = data["modulesProperties"];
    for (json::iterator it = modulesProperties.begin(); it != modulesProperties.end(); ++it) {
        std::string value;

        if (it.value().is_string())
            value = it.value();
        else if (it.value().is_number_float()) {
            float f = it.value();
            value = std::to_string(f);
        }
        else if (it.value().is_number_integer()) {
            int i = it.value();
            value = std::to_string(i);
        }

        _modulesProperties[it.key()] = value;
    }
}

uint16_t ZiaConfig::port() const {
    return _port;
}

std::string const &ZiaConfig::address() const {
    return _address;
}

std::string const &ZiaConfig::modulesPath() const {
    return _modulesPath;
}

ServerConfig::Platform ZiaConfig::platform() const {
#ifdef _WIN32
   return ServerConfig::Platform::Windows;
#elif __linux__
    return ServerConfig::Platform::Linux;
#elif __unix__
    // Unix
#elif defined(_POSIX_VERSION)
    return ServerConfig::Platform::Linux;
#else
#   return ServerConfig::Platform::Macos;
#endif
}

std::string ZiaConfig::platform_str() const {
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

std::string const &ZiaConfig::serverName() const {
    return _serverName;
}

std::string const &ZiaConfig::serverVersion() const {
    return _serverVersion;
}

std::unordered_map<std::string, std::string> const &ZiaConfig::modulesProperties() const {
    return _modulesProperties;
}
