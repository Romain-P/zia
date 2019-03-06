#pragma once

#include <iostream>
#include <string>
#include <Zia/API.hpp>

#include "RequestHandler.hpp"

class PhpModule : public Zia::API::Module
{
public:
    const std::string& getName() const override {
        return _name;
    }

    Zia::API::RequestHandler::pointer newRequestHandler() override {
        return std::make_shared<PhpRequestHandler>();
    }

private:
    std::string _name = "php";
};
