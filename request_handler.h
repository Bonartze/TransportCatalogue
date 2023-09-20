#include "svg.h"
#include "transport_catalogue.h"
#include "domain.h"
#include "json.h"
#include <map>
#include <vector>

#pragma once

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * В качестве источника для идей предлагаем взглянуть на нашу версию обработчика запросов.
 * Вы можете реализовать обработку запросов способом, который удобнее вам.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */
namespace request {
    using BusStat = std::unordered_map<size_t, RouteImitation::Bus *>;

    class RequestHandler {
    public:

        //  RequestHandler(const RouteImitation::TransportCatalogue &db, const svg::renderer::MapRenderer &renderer);

        RequestHandler() = delete;

        explicit RequestHandler(RouteImitation::TransportCatalogue &);

        void AddBusStat(const std::string &,                                          //Facade
                        std::variant<std::vector<json::Node>,
                                std::string> &, bool);

        void AddStopState(const std::string &, Geographic::Coordinates,
                          const std::map<std::string, json::Node> &);

        // Возвращает информацию о маршруте (запрос Bus)

        void GetStopsByBus(const std::string &, int, std::stringstream&, std::vector<json::Node>&);

        // Возвращает маршруты, проходящие через
        void GetBusesByStop(const std::string &, int, std::stringstream &, std::vector<json::Node>&) const;

        // Этот метод будет нужен в следующей части итогового проекта
        //  svg::Document RenderMap() const;


    private:
        // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
        RouteImitation::TransportCatalogue &db_;
        //const renderer::MapRenderer &renderer_;
    };
}
