cmake_minimum_required(VERSION 3.26)
project(TransportCatalogue)

set(CMAKE_CXX_STANDARD 17)

add_executable(TransportCatalogue
        json.cpp main.cpp
        svg.cpp
        transport_catalogue.cpp domain.cpp
        json_reader.cpp map_renderer.cpp
        request_handler.cpp
        JsonTests.cpp
        SVGtests.cpp)

