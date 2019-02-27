//
// Created by romain on 2/27/19.
//

#ifndef ZIA_SERVERCONFIG_H
#define ZIA_SERVERCONFIG_H

#include "zia.h"

class ServerConfig {
public:
    explicit ServerConfig(std::string const &configPath = "config.json");

    enum os {
        NOT_SUPPORTED,
        WINDOWS,
        LINUX
    };

    bool load();

    uint16_t port() const;
    std::string const &address() const;
    std::string const &modulesPath() const;
    os platform() const;
    std::string platform_str() const;

private:
    void loadProperties();

    uint16_t _port = 80;
    std::string _address = "127.0.0.1";
    std::string _modulesPath = "modules";
    std::string _configPath;
};


#endif //ZIA_SERVERCONFIG_H
