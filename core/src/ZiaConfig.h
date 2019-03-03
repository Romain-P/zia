//
// Created by romain on 2/27/19.
//

#ifndef ZIA_SERVERCONFIG_H
#define ZIA_SERVERCONFIG_H

#include "zia.h"
#include <unordered_map>
#include <nlohmann/json.hpp>

class ZiaConfig {
public:
    using json = nlohmann::json;
    using module_path = std::string;
    using module_priority = ssizet;

    explicit ZiaConfig(std::string const &configPath = "config.json");

    bool load();

    std::string const &serverName() const;
    std::string const &serverVersion() const;
    uint16_t port() const;
    std::string const &address() const;
    std::string const &modulesPath() const;
    ServerConfig::Platform platform() const;
    std::string platform_str() const;
    uint16_t poolSize() const;
    uint32_t networkReadBuffer() const;
    std::vector<std::pair<module_path, module_priority>> const &modules() const;
    std::unordered_map<std::string, std::string> const &modulesProperties() const;

private:
    void loadProperties();
    void loadModules(json const &data);
    void loadModulesProperties(json const &data);

    std::string _serverName = "zia";
    std::string _serverVersion = "1.0";
    uint16_t _port = 80;
    std::string _address = "127.0.0.1";
    uint16_t _poolSize = 1;
    uint32_t _networkReadBuffer = 8096;
    std::string _modulesPath = "modules";
    std::string _configPath;
    std::vector<std::pair<module_path, module_priority>> _modules;
    std::unordered_map<std::string, std::string> _modulesProperties;
};


#endif //ZIA_SERVERCONFIG_H
