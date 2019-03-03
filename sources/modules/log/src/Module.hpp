/*
** EPITECH PROJECT, 2019
** CPP_zia_2018
** File description:
** [log module] Module definition
*/

#pragma once

#include <iostream>
#include <string>
#include <Zia/API.hpp>

#include "RequestHandler.hpp"

namespace Zia {
namespace Modules {
namespace Log {

class Module : public API::Module
{
public:
    const std::string& getName() const override
    {
        return _name;
    }

    API::RequestHandler::pointer newRequestHandler() override
    {
        return std::make_shared<RequestHandler>();
    }

private:
    std::string _name = "log";
};

}
}
}
