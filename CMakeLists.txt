cmake_minimum_required(VERSION 3.26)
project(TransportManual)

set(CMAKE_CXX_STANDARD 17)

add_executable(TransportManual main.cpp
        transport_catalogue.cpp
)