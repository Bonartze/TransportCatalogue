#include <string>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include "geo.h"

#pragma once


namespace RouteImitation {
    struct Stop {
        Coordinates coordinates;
        std::string name;

        explicit Stop(Coordinates &coord, std::string &nm) :
                coordinates(coord), name(nm) {}

        Stop() : coordinates{0.0, 0.0} {} // Initialize coordinates with default values

        bool operator==(const Stop &other) const;
    };


    class HashStop {
    public:
        std::size_t operator()(const Stop &stop) const;
    };

    struct Bus {
        size_t stops_number;
        std::unordered_set<Stop, HashStop> unique_stops; // unique stops
        double route_length_direct;  // direct route length
        size_t route_length_computed; // real route length
        double curvature;
        std::deque<Stop *> routes_;   // imitation of route
        char type;

        Bus() : stops_number(0), unique_stops({}),
                route_length_direct(0.0), routes_({}),
                type('u'), route_length_computed(0.0) {}
    };

    struct PairHash {
        std::size_t operator()(const std::pair<Stop *, Stop *> &p) const;
    };


    class TransportCatalogue {
    private:
        std::unordered_map<std::string, Stop> stops;  // O(1) all operation average
        std::unordered_map<size_t, Bus *> routes;
        std::unordered_map<Stop *, std::unordered_set<size_t>> buses_in_stops;   // buses with all their stops
        std::unordered_map<std::pair<Stop *, Stop *>, size_t, PairHash> stop_distances;    // distances between stops
    public:
        void AddStop(std::string &, Coordinates);   // add stop as member of class's field in unordered_map

        void AddStopDistances(std::string &, std::string &, size_t);

        std::unordered_set<size_t> &GetBusesInStop(std::string &);

        void AddTag(size_t, char c);

        std::unordered_map<size_t, Bus *> &GetRoutes();

        std::unordered_map<std::string, Stop> &GetStops();

        void AddBusRouteStop(size_t, std::string &);
    };
}