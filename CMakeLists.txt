cmake_minimum_required(VERSION 3.14)
project(NTWK LANGUAGES CXX)

add_library(ntwk STATIC
    src/websocket.cpp
    src/https_socket.cpp
)

# Find Dependencies
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.66 REQUIRED COMPONENTS system)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

target_link_libraries(ntwk
    PRIVATE
        Boost::system
        OpenSSL::Crypto
        OpenSSL::SSL
)

target_include_directories(ntwk
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(ntwk
    PRIVATE
        cxx_std_17
)

target_compile_options(ntwk
    PRIVATE
        -Wall
        -Wextra
)
