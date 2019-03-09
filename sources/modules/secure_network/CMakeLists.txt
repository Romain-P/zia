add_library(secure_network MODULE
        src/factory.cpp
)

set_target_properties(secure_network PROPERTIES
        PREFIX "zia-"
)

find_package(OpenSSL REQUIRED)

target_link_libraries(secure_network
        CONAN_PKG::OpenSSL
        CONAN_PKG::boost
        ZiaModuleAPISpec
)