#include "json_reader.h"
#include <fstream>

namespace HandlingGettingRequests {
    FillingCatalogue::FillingCatalogue(RouteImitation::TransportCatalogue &db) :
            db_(db), rh(db_), parsed_data({}), drawing(renderer::DrawRoute(db.GetStops(), db.GetRoutes())) {}

    void FillingCatalogue::FillFileData(const std::string &file_name) {
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
                        rh.AddStopState(temp_map.at("name").AsString(),             //Filling stops fields
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
                        rh.AddBusStat(description_request.AsMap().at("name").AsString(), // Filling buses fields
                                      stopsVariant, description_request.AsMap().at("is_roundtrip").AsBool());
                    }
                }
            } else if (key_request == "stat_requests") {  //Check request with further responds
                for (auto &request: value.AsArray()) {
                    if (request.AsMap().at("type").AsString() == "Stop") {
                        rh.GetBusesByStop(request.AsMap().at("name").AsString(), request.AsMap().at("id").AsInt(),
                                          output_json_data, json_out_data);
                    } else if (request.AsMap().at("type").AsString() == "Bus") {
                        rh.GetStopsByBus(request.AsMap().at("name").AsString(), request.AsMap().at("id").AsInt(),
                                         output_json_data, json_out_data);
                    }
                }
            } else if (key_request == "render_settings") {  //Setting up the setting
                auto CopyNodeInStd = [](auto &vecNodes, auto &vecInt) {
                    for (auto &node: vecNodes)
                        vecInt.push_back(node.AsDouble());
                };
                auto &p = drawing.GetParams();
                drawing.SetAll(db_.GetStops(), db_.GetRoutes());
                for (const auto &[param_key, param_value]: value.AsMap()) {
                    if (param_key == "width")
                        p.width = param_value.AsInt();
                    else if (param_key == "height")
                        p.height = param_value.AsInt();
                    else if (param_key == "padding")
                        p.padding = param_value.AsInt();
                    else if (param_key == "stop_radius")
                        p.stop_radius = param_value.AsInt();
                    else if (param_key == "line_width")
                        p.line_width = param_value.AsInt();
                    else if (param_key == "bus_label_font_size")
                        p.bus_label_font_size = param_value.AsInt();
                    else if (param_key == "bus_label_offset") {
                        CopyNodeInStd(param_value.AsArray(), p.bus_label_offset);
                    } else if (param_key == "stop_label_font_size")
                        p.stop_label_font_size = param_value.AsInt();
                    else if (param_key == "stop_label_offset") {
                        CopyNodeInStd(param_value.AsArray(), p.stop_label_offset);
                    } else if (param_key == "underlayer_color")
                        CopyNodeInStd(param_value.AsArray(), p.underlayer_color);
                }
            }
        }
    }


    void FillingCatalogue::DisplayResultRequest(const std::string &file_name) {
        drawing.Draw(file_name);
        json::Print(json::Document(json_out_data), output_json_data);
        std::cout<<output_json_data.str();
    }
}
