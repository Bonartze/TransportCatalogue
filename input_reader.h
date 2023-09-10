#include <string>
#include <iostream>
#include <sstream>
#include "transport_catalogue.h"

#pragma once
namespace InputData {
    namespace Parse {
        class Parser {
        public:
            void CoordinatesAndStopName(std::stringstream &);

            void BusNumberAndRoute(std::stringstream &);

            RouteImitation::TransportCatalogue &GetTransportCatalogue();

        private:
            static void CutSpaces(std::string &);

            RouteImitation::TransportCatalogue transportCatalogue;
        };
    }
    class FillTransportManual {
    public:
        void GetData();

        void CallTransportManual();

        RouteImitation::TransportCatalogue &GetTransportCataloguePars();

    private:

        Parse::Parser parser;
        std::string data;
    };
}