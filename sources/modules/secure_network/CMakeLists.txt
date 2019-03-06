add_library(secure_network MODULE
        src/factory.cpp
)

set_target_properties(secure_network PROPERTIES
        PREFIX "zia-"
)

find_package(OpenSSL REQUIRED)

target_link_libraries(secure_network
        OpenSSL::SSL
        CONAN_PKG::boost_asio
        ZiaModuleAPISpec
)