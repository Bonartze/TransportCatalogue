#pragma once

#include <sstream>
#include <iostream>
#include "transport_catalogue.h"
#include "json.h"
#include "map_renderer.h"
#include "request_handler.h"

namespace HandlingGettingRequests {
    class FillingCatalogue {
    public:

        explicit FillingCatalogue(RouteImitation::TransportCatalogue &);

        void FillFileData(const std::string &);

        void FillingData();

        void GetParsedJsonData();

        void DisplayResultRequest(const std::string &);

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