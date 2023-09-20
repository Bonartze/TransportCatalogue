#include "geo.h"
#include <string>
#include <unordered_set>
#include <deque>

#pragma once

namespace RouteImitation {

    struct Stop;

    class HashStop {
    public:
        std::size_t operator()(const Stop &stop) const;
    };

    struct Stop {
        Geographic::Coordinates coordinates;
        std::string name;

        explicit Stop(const Geographic::Coordinates &coord, const std::string &nm) :
                coordinates(coord), name(nm) {}

        Stop() : coordinates{0.0, 0.0} {} // Initialize coordinates with default values

        bool operator==(const Stop &other) const;
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
                type('u'), route_length_computed(0.0), curvature(0.0) {}
    };

    struct PairHash {
        std::size_t operator()(const std::pair<Stop *, Stop *> &p) const;
    };

}