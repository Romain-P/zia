//
// Created by romain on 2/27/19.
//

#include "ZiaConfig.h"
#include <fstream>
#include <iostream>

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

    if (data.find("poolSize") != data.end())
        _poolSize = data["poolSize"];

    if (data.find("networkReadBuffer") != data.end())
        _networkReadBuffer = data["networkReadBuffer"];

    loadModules(data);
    loadModulesProperties(data);
}

void ZiaConfig::loadModules(json const &data) {
    json modulesProperties = data["modules"];

    _modules.clear();
    for (json::iterator it = modulesProperties.begin(); it != modulesProperties.end(); ++it) {
        module_path const &path = it.key();
        module_priority priority = it.value();

        _modules.emplace_back(path, priority);
    }
}

void ZiaConfig::loadModulesProperties(const ZiaConfig::json &data) {
    json modulesProperties = data["modulesProperties"];

    _modulesProperties.clear();
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

uint16_t ZiaConfig::poolSize() const {
    return _poolSize;
}

uint32_t ZiaConfig::networkReadBuffer() const {
    return _networkReadBuffer;
}

const std::vector<std::pair<ZiaConfig::module_path, ZiaConfig::module_priority>> &ZiaConfig::modules() const {
    return _modules;
}
