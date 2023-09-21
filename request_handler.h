#include "svg.h"
#include "transport_catalogue.h"
#include "domain.h"
#include "json.h"
#include <map>
#include <vector>

#pragma once

namespace request {
    using BusStat = std::unordered_map<size_t, RouteImitation::Bus *>;

    class RequestHandler {
    public:

        RequestHandler() = delete;

        explicit RequestHandler(RouteImitation::TransportCatalogue &);

        void AddBusStat(const std::string &,                                          //Pattern Facade added
                        std::variant<std::vector<json::Node>,
                                std::string> &, bool);

        void AddStopState(const std::string &, Geographic::Coordinates,
                          const std::map<std::string, json::Node> &);


        void GetStopsByBus(const std::string &, int, std::stringstream&, std::vector<json::Node>&);

        void GetBusesByStop(const std::string &, int, std::stringstream &, std::vector<json::Node>&) const;



    private:
        RouteImitation::TransportCatalogue &db_;
    };
}
