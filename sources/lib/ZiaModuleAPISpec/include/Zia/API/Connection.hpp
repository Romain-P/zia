/*
** EPITECH PROJECT, 2019
** CPP_zia_2018
** File description:
** [file description here]
*/

#pragma once

#include <string>

namespace Zia {
namespace API {

/*
 * CONNECTION OBJECT
 *
 * Contains all information about the current connection.
 */
struct Connection
{

    /*
     * Remote address (ip):
     *     127.0.0.1
     */
    std::string addr;

    /*
     * Remote port:
     *     4242
     */
    unsigned short port;
};

}
}
