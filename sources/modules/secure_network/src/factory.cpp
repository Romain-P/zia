#include <iostream>
#include <Zia/API.hpp>

#include "Module.hpp"

ZIA_API_EXPORT Zia::API::Module::pointer factory() {
    return new SecureNetworkModule();
}

ZIA_API_EXPORT void recycler(Zia::API::Module::pointer instance) {
    delete instance;
}
