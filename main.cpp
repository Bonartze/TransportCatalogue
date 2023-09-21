#include "json_reader.h"
#include "transport_catalogue.h"

int main() {
    RouteImitation::TransportCatalogue tc;
    HandlingGettingRequests::FillingCatalogue fc(tc);
    fc.FillFileData("test.txt");
    fc.GetParsedJsonData();
    fc.FillingData();
    fc.DisplayResultRequest("result.svg");
    return 0;
}