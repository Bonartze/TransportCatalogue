#include "Json_parser_src/json_reader.h"
#include "Mapping_src/transport_catalogue.h"

int main() {
    RouteImitation::TransportCatalogue tc;
    HandlingGettingRequests::FillingCatalogue fc(tc);
    fc.FillFileData("test.json");
    fc.GetParsedJsonData();
    fc.FillingData();
    fc.DisplayResultRequest("result.svg");
    return 0;
}
