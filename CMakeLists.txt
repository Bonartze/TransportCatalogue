cmake_minimum_required(VERSION 3.25)
project(TransportCatalogue)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)


FetchContent_MakeAvailable(googletest)

add_executable(TransportCatalogue
        json.cpp main.cpp
        svg.cpp
        transport_catalogue.cpp domain.cpp
        json_reader.cpp map_renderer.cpp
        request_handler.cpp
        JsonTests.cpp
        SVGtests.cpp)

target_link_libraries(TransportCatalogue PRIVATE gtest gtest_main)


