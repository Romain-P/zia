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

add_subdirectory(sources)

install(FILES sources/core/dist/config.json DESTINATION ${CMAKE_SOURCE_DIR}/generated)
install(FILES sources/modules/secure_network/dist/cert.crt DESTINATION ${CMAKE_SOURCE_DIR}/generated/tls)
install(FILES sources/modules/secure_network/dist/private.key DESTINATION ${CMAKE_SOURCE_DIR}/generated/tls)
install(TARGETS zia log secure_network unsecure_network
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/generated
        LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/generated/modules)

set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)

if (UNIX)
    message("Please follow the instructions to install php-cgi")
    execute_process(
            COMMAND bash "-c" "bash ${CMAKE_SOURCE_DIR}/sources/modules/php/dist/php-cgi/linux/install.sh"
            RESULT_VARIABLE
            RET
    )
    if(RET EQUAL "1")
        message(FATAL_ERROR "Error while installing php-cgi, please install it manually")
    endif()
endif(UNIX)