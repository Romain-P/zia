/*
** EPITECH PROJECT, 2019
** ZiaModuleAPISpec
** File description:
** Request spec
*/

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace Zia {
namespace API {

/*
 * REQUEST OBJECT
 *
 * Contains all information about the current request that was parsed.
 */
struct Request {

    /*
     * Request method:
     *     GET/POST/PUT/PATCH/DELETE/OPTIONS/HEAD/custom
     */
    std::string method;

    /*
     * Request uri:
     *     /path/to/index.php?query=string&is=not&parsed
     */
    std::string uri;

    /*
     * Request protocol:
     *     HTTP/1.1
     */
    std::string protocol;

    /*
     * Request headers:
     *     map of strings containing all parsed headers.
     *     if a header occurs multiple times, the last occurence is saved here.
     */
    std::unordered_map<std::string, std::string> headers;

    /*
     * Request body:
     *     Raw buffer containing request body. To be parsed by request handler.
     */
    std::vector<char> body;

    /*
     * Secure:
     *     true if the request has been made through ssl/tls
     */
    bool secure;
};

}
}
