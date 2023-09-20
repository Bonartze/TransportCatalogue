cmake_minimum_required(VERSION 3.26)
project(TransportCatalogue)

set(CMAKE_CXX_STANDARD 17)

add_executable(TransportCatalogue
        json.cpp TransportCatalogue/main.cpp
        svg.cpp
        transport_catalogue.cpp TransportCatalogue/domain.cpp
        json_reader.cpp TransportCatalogue/map_renderer.cpp
        request_handler.cpp TransportCatalogue/map_renderer.h)
