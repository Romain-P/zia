/*
** EPITECH PROJECT, 2019
** ZiaModuleAPISpec
** File description:
** Server config spec
*/

#pragma once

#include <string>
#include <unordered_map>

#include "Definitions.hpp"

namespace Zia {
namespace API {

/*
 * SERVER CONFIGURATION:
 *
 * This struct contains all configuration needed to initialize a module.
 * The user-provided config is transmitted in a string map.
 */
struct ServerConfig {

    enum class Platform {
        Linux,
        Windows,
        Macos
    };

    /*
     * Server name:
     *     can be set to system's hostname
     */
    std::string name;

    /*
     * Server version:
     *     can be used to require specific version from module
     */
    std::string version;

    /*
     * Server platform:
     *     which platform the server is running on
     */
    Platform platform;

    /*
     * Server config:
     *     A map of config values provided by the user.
     *     The config cannot have depth. Instead, it should be "emulated" using
     *     dot notation. for example "this.is.deep"
     */
    std::unordered_map<std::string, std::string> config;

    /*
     * API spec version:
     *     Do not touch this, it is already set by default.
     */
    std::string apispec_version = Definitions::VERSION;
};

}
}
