#include "input_reader.h"
#include "stat_reader.h"

int main() {
    InputData::FillTransportManual input;
    input.GetData();
    input.CallTransportManual();
    Display::Data::RequestData(input);
    return 0;
}