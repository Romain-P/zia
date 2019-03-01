cmake_minimum_required(VERSION 3.10)
project(zia CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
INCLUDE_DIRECTORIES(${CONAN_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g3")
set(CMAKE_BUILD_TYPE Release)

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)
add_subdirectory(lib/ZiaModuleAPISpec)

set(FILES
        src/zia.h
        src/main.cpp
        src/ZiaConfig.cpp src/ZiaConfig.h
        src/Server.cpp src/Server.h
        src/Session.cpp src/Session.h
        src/Console.cpp src/Console.h src/RequestParser.cpp src/RequestParser.h)

add_executable(zia ${FILES})
target_link_libraries(zia CONAN_PKG::boost_asio CONAN_PKG::jsonformoderncpp ZiaModuleAPISpec)
