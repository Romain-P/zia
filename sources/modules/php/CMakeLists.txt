add_library(php MODULE
        src/factory.cpp
)

set_target_properties(php PROPERTIES
        PREFIX "zia-"
)
target_link_libraries(php
        CONAN_PKG::boost_asio
        ZiaModuleAPISpec
)