add_library(log MODULE
        src/factory.cpp
)
set_target_properties(log PROPERTIES
        PREFIX "zia-"
)
target_link_libraries(log
        CONAN_PKG::boost_asio
        ZiaModuleAPISpec
)