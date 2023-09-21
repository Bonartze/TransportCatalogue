#include "json_reader.h"
#include "transport_catalogue.h"
#include "SVGtests.hpp"
#include "JsonTests.hpp"

int main() {
    JsonTest::RunAllJsonTests();
    SVGtest::RunAllTests();
    RouteImitation::TransportCatalogue tc;
    HandlingGettingRequests::FillingCatalogue fc(tc);
    fc.FillFileData("test.txt");
    fc.GetParsedJsonData();
    fc.FillingData();
    fc.DisplayResultRequest("result.svg");
    return 0;
}