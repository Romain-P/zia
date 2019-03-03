cmake_minimum_required(VERSION 3.10)

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
install(TARGETS zia log
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/generated
        LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/generated/modules)