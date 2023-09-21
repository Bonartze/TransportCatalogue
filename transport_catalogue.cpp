#include "transport_catalogue.h"

namespace RouteImitation {

    void TransportCatalogue::AddBusRouteStop(const std::string& number, const std::string &stop_name) {
        buses_in_stops[&stops[stop_name]].insert(number);
        if (routes[number]->type == 'c')
            routes[number]->stops_number++;
        else {
            routes[number]->stops_number = routes[number]->routes_.size() * 2 + 1;
        }
        routes[number]->unique_stops.insert(stops[stop_name]); //for precise number of unique stops
        if (!routes[number]->routes_.empty()) {
            auto last_stop = routes[number]->routes_.back();
            if (routes[number]->type == 'c')
                routes[number]->route_length_direct += ComputeDistance(last_stop->coordinates,
                                                                       stops[stop_name].coordinates);
            else
                routes[number]->route_length_direct +=
                        2 * ComputeDistance(last_stop->coordinates, stops[stop_name].coordinates);
            if (routes[number]->type == 'c')
                routes[number]->route_length_computed +=
                        stop_distances.find({last_stop, &stops[stop_name]}) == end(stop_distances) ?
                        stop_distances[{&stops[stop_name], last_stop}] : stop_distances[{last_stop, &stops[stop_name]}];
            else {
                if (stop_distances.find({last_stop, &stops[stop_name]}) == end(stop_distances)) {
                    routes[number]->route_length_computed += 2 * stop_distances[{&stops[stop_name], last_stop}];
                } else {
                    if (stop_distances.find({&stops[stop_name], last_stop}) != end(stop_distances)) {
                        routes[number]->route_length_computed += stop_distances[{&stops[stop_name], last_stop}] +
                                                                 stop_distances[{last_stop, &stops[stop_name]}];
                    } else
                        routes[number]->route_length_computed += 2 * stop_distances[{last_stop, &stops[stop_name]}];
                }
            }

        }

        routes[number]->routes_.
                push_back(&stops[stop_name]);
        if (routes[number]->route_length_direct != 0)
            routes[number]->
                    curvature =
                    (double) routes[number]->route_length_computed / routes[number]->route_length_direct;
    }

    void TransportCatalogue::AddTag(const std::string& number, char c) {
        routes[number] = new Bus();
        routes[number]->type = c;
    }

    void
    TransportCatalogue::AddStopDistances(const std::string &name_from, const std::string &name_to, size_t distance) {
        stops[name_to].name = name_to;
        stop_distances[{&stops[name_from], &stops[name_to]}] = distance;
    }

    void TransportCatalogue::AddStop(const std::string &name, Geographic::Coordinates coordinates) {
           stops[name] = Stop(coordinates, name);
        //stops.insert({name, Stop(coordinates, name)});
    }

    std::unordered_map<std::string, Bus *> &TransportCatalogue::GetRoutes() {
        return routes;
    }


    std::unordered_set<std::string> &TransportCatalogue::GetBusesInStop(const std::string &stop_name) {
        return buses_in_stops[&stops[stop_name]];
    }

    std::unordered_map<std::string, Stop> &TransportCatalogue::GetStops() {
        return stops;
    }


}