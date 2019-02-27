//
// Created by romain on 2/27/19.
//

#ifndef ZIA_SERVERCONFIG_H
#define ZIA_SERVERCONFIG_H

#include "zia.h"
#include <unordered_map>

class ZiaConfig {
public:
    explicit ZiaConfig(std::string const &configPath = "config.json");

    bool load();

    std::string const &serverName() const;
    std::string const &serverVersion() const;
    uint16_t port() const;
    std::string const &address() const;
    std::string const &modulesPath() const;
    ServerConfig::Platform platform() const;
    std::string platform_str() const;
    std::unordered_map<std::string, std::string> const &modulesProperties() const;

private:
    void loadProperties();

    std::string _serverName = "zia";
    std::string _serverVersion;
    uint16_t _port = 80;
    std::string _address = "127.0.0.1";
    std::string _modulesPath = "modules";
    std::string _configPath;
    std::unordered_map<std::string, std::string> _modulesProperties;
};


#endif //ZIA_SERVERCONFIG_H
