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
        void AddStop(const std::string &, Geographic::Coordinates);   // add stop as member of class's field in unordered_map

        void AddStopDistances(const std::string &, const std::string &, size_t);


         std::unordered_set<size_t> &GetBusesInStop(const std::string &);

        void AddTag(size_t, char c);

        std::unordered_map<size_t, Bus *> &GetRoutes();

        std::unordered_map<std::string, Stop> &GetStops();

        void AddBusRouteStop(size_t, const std::string &);

    private:
        std::unordered_map<std::string, Stop> stops;  // O(1) all operation average
        std::unordered_map<size_t, Bus *> routes;
        std::unordered_map<Stop *, std::unordered_set<size_t>> buses_in_stops;   // buses with all their stops
        std::unordered_map<std::pair<Stop *, Stop *>, size_t, PairHash> stop_distances;    // distances between stops
    };
}