cmake_minimum_required(VERSION 3.13)
project(zia CXX)

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
INCLUDE_DIRECTORIES(${CONAN_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g3")
set(CMAKE_BUILD_TYPE Release)

add_subdirectory(lib/ZiaModuleAPISpec)
add_subdirectory(core)
add_subdirectory(modules)

install(FILES core/dist/config.json DESTINATION ${CMAKE_SOURCE_DIR}/generated)
install(TARGETS zia log unsecure_network
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/generated
        LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/generated/modules)