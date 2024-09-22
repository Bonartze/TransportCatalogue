#include "domain.h"
namespace RouteImitation {
    std::size_t PairHash::operator()(const std::pair<Stop *, Stop *> &p) const {  //hashing pairs<Stop*, Stop*>
        std::hash<const void *> hasher;
        return hasher(p.first) ^ hasher(p.second);
    }


    bool Stop::operator==(const Stop &other) const {
        return std::tie(coordinates.lng, coordinates.lat) == std::tie(other.coordinates.lng, other.coordinates.lat);
    }

    size_t HashStop::operator()(const Stop &stop) const {
        return 37 + 37 * (int) stop.coordinates.lng;
    }
}

