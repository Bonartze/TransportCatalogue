#include <string>
#include <sstream>
#include <iostream>
#include "transport_catalogue.h"
#include "input_reader.h"

namespace Display {
    class Data {
    public:
        static void RequestData(InputData::FillTransportManual &);
    };
}