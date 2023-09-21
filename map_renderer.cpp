#include "map_renderer.h"
#include <fstream>

namespace renderer {
    bool IsZero(double value) {
        return std::abs(value) < EPSILON;
    }

    DrawRoute::DrawRoute(std::unordered_map<std::string, RouteImitation::Stop> &AllStops,
                         std::unordered_map<std::string, RouteImitation::Bus *> &route)
            : routes(route), stops_coordinates(AllStops) {
        for (auto &[bus_stop, bus_stop_coordinates]: stops_coordinates)
            AllCords.emplace_back(bus_stop_coordinates.coordinates);
    }

    void DrawRoute::SetAll(std::unordered_map<std::string, RouteImitation::Stop> &AllStops,
                           std::unordered_map<std::string, RouteImitation::Bus *> &route) {
        routes = route;
        stops_coordinates = AllStops;
        for (auto &[bus_stop, bus_stop_coordinates]: stops_coordinates)
            AllCords.emplace_back(bus_stop_coordinates.coordinates);
    }

    void DrawRoute::Draw(const std::string &file_name) {
        SphereProjector sp(AllCords.begin(), AllCords.end(), params.width, params.height, params.padding); //for mapping
        for (const auto &[bus_number, bus]: routes) {   // lines in route, take line one for one to not draw incorrect set, so used nested loops
            auto pl = svg::Polyline();
            for (const auto &stop: bus->routes_) {
                pl.AddPoint(sp(stop->coordinates)).SetStrokeColor("black").SetStrokeWidth(
                        params.line_width).SetStrokeLineCap(svg::StrokeLineCap::ROUND).SetStrokeColor(
                        "green").SetStrokeLineJoin(svg::StrokeLineJoin::ROUND).SetFillColor("none");
            }
            doc.Add(pl);
        }
        for (const auto &[stop_name, stop_coordinate_]: stops_coordinates) //Stops in route
            doc.Add(svg::Circle().SetRadius(
                    params.stop_radius).SetCenter(   // Set up the color and coordinates using map method and drawing circles (bus stops on the picture)
                    sp(stop_coordinate_.coordinates)).SetFillColor("white"));
        for (const auto &[stop_name, stop_coordinate_]: stops_coordinates) {       //Adding text on picture (names of stops)
            doc.Add(svg::Text().SetData(stop_name).SetPosition(
                    sp(stops_coordinates[stop_name].coordinates)).SetFontSize(params.bus_label_font_size).SetOffset(
                    svg::Point(params.bus_label_offset.at(0), params.bus_label_offset.at(1))).SetStrokeLineJoin(
                    svg::StrokeLineJoin::ROUND).SetStrokeLineCap(svg::StrokeLineCap::ROUND));
        }
        for (const auto &[bus_number, bus]: routes) {             //Adding text on picture (number of buses)
            if (bus->routes_.back()->coordinates != bus->routes_.front()->coordinates) {
                doc.Add(svg::Text().SetPosition(sp(bus->routes_.front()->coordinates)).SetData(
                        bus_number + " start").SetFontSize(params.line_width).SetStrokeColor("Red").SetFillColor(
                        "Red"));
                doc.Add(svg::Text().SetPosition(sp(bus->routes_.back()->coordinates)).SetData(
                        bus_number + " end").SetFontSize(params.line_width).SetStrokeColor("Red").SetFillColor("Red"));
            } else
                doc.Add(svg::Text().SetPosition(sp(bus->routes_.front()->coordinates)).SetData(
                        bus_number + " start - end").SetFontSize(params.bus_label_font_size).SetStrokeColor(
                        "Red").SetFillColor("Red"));
        }
        std::fstream f(file_name);
        doc.Render(f);
    }
}