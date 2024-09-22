#pragma once

#include <sstream>
#include <iostream>
#include "../Mapping_src/transport_catalogue.h"
#include "json.h"
#include "../Mapping_src/map_renderer.h"
#include "../Mapping_src/request_handler.h"

namespace HandlingGettingRequests {
    class FillingCatalogue {
    public:

        explicit FillingCatalogue(RouteImitation::TransportCatalogue &);

        void FillFileData(const std::string &);  //Filling stream by data of the input file

        void GetParsedJsonData();   //Parsing data using Json library

        void FillingData();   // Using Json library fill data in transport catalogue

        void DisplayResultRequest(const std::string &); // Just drawing routes in a stream(In this project in file)

    private:
        renderer::DrawRoute drawing;
        std::stringstream input_json_data;
        RouteImitation::TransportCatalogue &db_;
        request::RequestHandler rh;
        json::Document parsed_data;
        std::stringstream output_json_data;
        std::vector<json::Node> json_out_data;
    };
}