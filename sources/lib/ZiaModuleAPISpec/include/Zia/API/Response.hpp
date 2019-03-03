/*
** EPITECH PROJECT, 2019
** ZiaModuleAPISpec
** File description:
** Response spec
*/

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace Zia {
namespace API {

/*
 * RESPONSE OBJECT
 *
 * Contains all information about the current response that will be serialized.
 */
struct Response {

    /*
     * Response protocol:
     *     HTTP/1.1
     */
    std::string protocol;

    /*
     * Response status code:
     *     200/400/400/etc
     */
    unsigned short status_code;

    /*
     * Response status message:
     *     Not Found
     */
    std::string status_message;

    /*
     * Response headers:
     *     map of strings containing all headers to be sent
     */
    std::unordered_map<std::string, std::string> headers;

    /*
     * Response body:
     *     Raw buffer containing response body.
     */
    std::vector<char> body;
};

}
}
