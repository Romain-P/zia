add_library(unsecure_network MODULE
        src/factory.cpp
)

set_target_properties(unsecure_network PROPERTIES
        PREFIX "zia-"
)
target_link_libraries(unsecure_network
        CONAN_PKG::boost
        ZiaModuleAPISpec
)