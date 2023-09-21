#include <string>
#include <deque>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include "domain.h"

#pragma once


namespace RouteImitation {
    class TransportCatalogue {
    public:
        void
        AddStop(const std::string &, Geographic::Coordinates);   // add stop as member of class's field in unordered_map

        void AddStopDistances(const std::string &, const std::string &, size_t);


        std::unordered_set<std::string> &GetBusesInStop(const std::string &);

        void AddTag(const std::string&, char c);

        std::unordered_map<std::string, Bus *> &GetRoutes();

        std::unordered_map<std::string, Stop> &GetStops();

        void AddBusRouteStop(const std::string &, const std::string &);

    private:
        std::unordered_map<std::string, Stop> stops;  // O(1) all operation average
        std::unordered_map<std::string, Bus *> routes;
        std::unordered_map<Stop *, std::unordered_set<std::string>> buses_in_stops;   // buses with all their stops
        std::unordered_map<std::pair<Stop *, Stop *>, size_t, PairHash> stop_distances;    // distances between stops
    };
}