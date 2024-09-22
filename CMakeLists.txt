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

add_executable(transport_catalogue
        Json_parser_src/json.cpp
        main.cpp
        Mapping_src/svg.cpp
        Mapping_src/transport_catalogue.cpp Geography_src/domain.cpp
        Json_parser_src/json_reader.cpp Mapping_src/map_renderer.cpp
        Mapping_src/request_handler.cpp
        Autotests/JsonTests.cpp
        Autotests/SVGtests.cpp
)

add_executable(run_tests
        Json_parser_src/json.cpp
        Mapping_src/svg.cpp
        Mapping_src/transport_catalogue.cpp Geography_src/domain.cpp
        Json_parser_src/json_reader.cpp Mapping_src/map_renderer.cpp
        Mapping_src/request_handler.cpp
        Autotests/JsonTests.cpp
        Autotests/SVGtests.cpp
        Autotests/gtest_main.cpp
)


target_link_libraries(transport_catalogue PRIVATE GTest::gtest_main)

target_link_libraries(run_tests PRIVATE GTest::gtest_main)

