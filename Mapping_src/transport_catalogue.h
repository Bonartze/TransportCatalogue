#include <string>
#include <deque>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include "../Geography_src/domain.h"

#pragma once


namespace RouteImitation {
    class TransportCatalogue {
    public:
        void AddStop(const std::string &, Geographic::Coordinates);

        // Add stop as member of class's field in unordered_map

        void AddStopDistances(const std::string &, const std::string &,
                              size_t); // Add pair of the stops and distance between, first parameter is stop from,
        // the second one is to, third is distance


        std::unordered_set<std::string> &GetBusesInStop(
            const std::string &);

        // return reference to unordered_set with all buses that come across the bus (parameter)

        void AddTag(const std::string &, char c); // Add tag to route is if it is cycle or uncycle

        std::unordered_map<std::string, Bus *> &GetRoutes(); // Return reference to all the routes by bus's name

        std::unordered_map<std::string, Stop> &GetStops(); // Return all the bus stops

        void AddBusRouteStop(const std::string &,
                             const std::string &);

        // Push in the route of bus new bus stop and calculate all parameters instantly

    private:
        std::unordered_map<std::string, Stop> stops; // O(1) all operation average
        std::unordered_map<std::string, Bus *> routes;
        std::unordered_map<Stop *, std::unordered_set<std::string> > buses_in_stops; // buses with all their stops
        std::unordered_map<std::pair<Stop *, Stop *>, size_t, PairHash> stop_distances; // distances between stops
    };
}
