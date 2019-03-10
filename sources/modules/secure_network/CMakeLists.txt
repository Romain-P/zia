add_library(secure_network MODULE
        src/factory.cpp
)

set_target_properties(secure_network PROPERTIES
        PREFIX "zia-"
)

target_link_libraries(secure_network
        CONAN_PKG::OpenSSL
        CONAN_PKG::boost
        ZiaModuleAPISpec
)