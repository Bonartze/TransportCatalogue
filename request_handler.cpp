#include "request_handler.h"

namespace request {
    RequestHandler::RequestHandler(RouteImitation::TransportCatalogue &db) : db_(db) {}


    void RequestHandler::AddStopState(const std::string &name_from, Geographic::Coordinates coordinates,
                                      const std::map<std::string, json::Node> &road_distances) {
        for (auto &[name_to, distance]: road_distances)
            db_.AddStopDistances(name_from, name_to, distance.AsDouble());
        db_.AddStop(name_from, coordinates);
    }


    void RequestHandler::AddBusStat(const std::string &number,                                          //Facade
                                    std::variant<std::vector<json::Node>,
                                            std::string> &stops, bool is_roundtrip) {
        db_.AddTag(stoul(number), is_roundtrip ? 'c' : 'u');
        if (std::holds_alternative<std::string>(stops)) {
            db_.AddBusRouteStop(stoul(number), std::get<std::string>(stops));
        } else {
            for (auto &stop: std::get<std::vector<json::Node>>(stops)) {
                db_.AddBusRouteStop(stoul(number), stop.AsString());
            }
        }
    }

    void RequestHandler::GetBusesByStop(const std::string &stop_name, int id, std::stringstream &buses,
                                        std::vector<json::Node> &output_json_format) const {
        auto temp = db_.GetBusesInStop(stop_name);
        std::vector<json::Node> tmp;
        for (const auto &i: temp) {
            tmp.push_back(std::move(json::Node(int(i))));
        }
        output_json_format.push_back(json::Node({{"buses",      json::Node{tmp}},
                                                 {"request_id", json::Node{id}}}));
    }

    void RequestHandler::GetStopsByBus(const std::string &number, int id, std::stringstream &stops,
                                       std::vector<json::Node> &output_json_format) {
        std::vector<json::Node> v;
        output_json_format.push_back(json::Node{
                {{{"curvature", json::Node(db_.GetRoutes().at(stoul(number))->curvature)},
                  {"request_id", json::Node(id)},
                  {"route_length", json::Node((int) db_.GetRoutes().at(stoul(number))->route_length_computed)},
                  {"stop_count", json::Node((int) db_.GetRoutes().at(stoul(number))->stops_number)},
                  {"unique_stop_count", json::Node((int) db_.GetRoutes().at(stoul(number))->unique_stops.size())}}}});
    }
}