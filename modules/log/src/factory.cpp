/*
** EPITECH PROJECT, 2019
** CPP_zia_2018
** File description:
** [log module] Factory & recycler definition
*/

#include <iostream>
#include <Zia/API.hpp>

#include "Module.hpp"

ZIA_API_EXPORT Zia::API::Module::pointer factory()
{
    return new Zia::Modules::Log::Module;
}

ZIA_API_EXPORT void recycler(Zia::API::Module::pointer instance)
{
    delete instance;
}
