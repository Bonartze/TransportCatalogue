#include "transport_catalogue.h"

namespace RouteImitation {

    bool Stop::operator==(const Stop &other) const {
        return ::std::tie(coordinates.lng, coordinates.lat) == std::tie(other.coordinates.lng, other.coordinates.lat);
    }

    size_t HashStop::operator()(const Stop &stop) const {
        return 37 + 37 * (int) stop.coordinates.lng;
    }

    void TransportCatalogue::AddBusRouteStop(size_t number, std::string &stop_name) {       // minus 1 for right answer
        buses_in_stops[&stops[stop_name]].insert(number);
        if (routes[number]->type == 'c')
            routes[number]->stops_number++;
        else {
            routes[number]->stops_number = routes[number]->unique_stops.size() * 2 + 1;
        }
        routes[number]->unique_stops.insert(stops[stop_name]); //for precise number of unique stops
        if (!routes[number]->routes_.empty()) {
            auto last_stop = routes[number]->routes_.back();
            if (routes[number]->type == 'c')
                routes[number]->route_length += ComputeDistance(last_stop->coordinates, stops[stop_name].coordinates);
            else
                routes[number]->route_length +=
                        2 * ComputeDistance(last_stop->coordinates, stops[stop_name].coordinates);


        }
        routes[number]->routes_.push_back(&stops[stop_name]);

    }

    void TransportCatalogue::AddTag(size_t number, char c) {
        routes[number] = new Bus();
        routes[number]->type = c;
    }

    void TransportCatalogue::AddStop(std::string &name, Coordinates coordinates) {
        stops[name] = Stop(coordinates, name);
    }

    std::unordered_map<size_t, Bus *> &TransportCatalogue::GetRoutes() {
        return routes;
    }


    std::unordered_set<size_t> &TransportCatalogue::GetBusesInStop(std::string &stop_name) {
        return buses_in_stops[&stops[stop_name]];
    }

    std::unordered_map<std::string, Stop> &TransportCatalogue::GetStops() {
        return stops;
    }

}