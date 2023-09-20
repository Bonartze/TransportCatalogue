#include "json_reader.h"
#include <fstream>

namespace HandlingGettingRequests {
    FillingCatalogue::FillingCatalogue(RouteImitation::TransportCatalogue &db) :
            db_(db), rh(db_), parsed_data({}) {}

    void FillingCatalogue::FillFileData(std::string file_name) {
        std::string line;
        std::fstream f;
        f.open(file_name);
        while (getline(f, line, '\n')) {
            input_json_data << line << '\n';
        }
    }


    void FillingCatalogue::GetParsedJsonData() {
        parsed_data = json::Load(input_json_data);
    }

    void FillingCatalogue::FillingData() {
        for (const auto &[key_request, value]: parsed_data.GetRoot().AsMap()) {
            if (key_request == "base_requests") {
                for (const auto &description_request: value.AsArray()) {
                    if (description_request.AsMap().at("type").AsString() == "Stop") {
                        auto temp_map = description_request.AsMap();
                        rh.AddStopState(temp_map.at("name").AsString(),
                                        {temp_map.at("latitude").AsDouble(), temp_map.at("longitude").AsDouble()},
                                        temp_map.at("road_distances").AsMap());
                    }
                }
                for (const auto &description_request: value.AsArray()) {
                    if (description_request.AsMap().at("type").AsString() == "Bus") {
                        std::variant<std::vector<json::Node>, std::string> stopsVariant;
                        if (description_request.AsMap().at("stops").IsArray())
                            stopsVariant = description_request.AsMap().at("stops").AsArray();
                        else
                            stopsVariant = description_request.AsMap().at("stops").AsString();
                        rh.AddBusStat(description_request.AsMap().at("name").AsString(),
                                      stopsVariant, description_request.AsMap().at("is_roundtrip").AsBool());
                    }
                }
            } else if (key_request == "stat_requests") {
                for (auto &request: value.AsArray()) {
                    if (request.AsMap().at("type").AsString() == "Stop") {
                        rh.GetBusesByStop(request.AsMap().at("name").AsString(), request.AsMap().at("id").AsInt(),
                                          output_json_data, json_out_data);
                    } else if (request.AsMap().at("type").AsString() == "Bus") {
                        rh.GetStopsByBus(request.AsMap().at("name").AsString(), request.AsMap().at("id").AsInt(),
                                         output_json_data, json_out_data);
                    }
                }
            }
        }
    }

    void FillingCatalogue::DisplayResultRequest() {
        json::Print(json::Document(json_out_data), output_json_data);
        std::cout << output_json_data.str() << std::endl;

    }
}