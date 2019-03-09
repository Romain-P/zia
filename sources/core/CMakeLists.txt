cmake_minimum_required(VERSION 3.13)

set(FILES
        src/zia.h
        src/main.cpp
        src/ZiaConfig.cpp src/ZiaConfig.h
        src/Server.cpp src/Server.h
        src/Session.cpp src/Session.h
        src/Console.cpp src/Console.h
        src/Network.cpp src/Network.h
        src/Modules.cpp src/Modules.h
        src/Http.h src/DynamicLibrary.h)

add_executable(zia ${FILES})
target_link_libraries(zia ${CMAKE_DL_LIBS} CONAN_PKG::boost CONAN_PKG::jsonformoderncpp ZiaModuleAPISpec)
