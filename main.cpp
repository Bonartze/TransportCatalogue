#include "json_reader.h"
#include "transport_catalogue.h"
#include "SVGtests.hpp"
#include "JsonTests.hpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    /*JsonTest::RunAllJsonTests();
    SVGtest::RunAllTests();
    RouteImitation::TransportCatalogue tc;
    HandlingGettingRequests::FillingCatalogue fc(tc);
    fc.FillFileData("test.txt");
    fc.GetParsedJsonData();
    fc.FillingData();
    fc.DisplayResultRequest("result.svg");*/
    return 0;
}