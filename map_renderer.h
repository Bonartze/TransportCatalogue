
#include "geo.h"
#include "svg.h"
#include "domain.h"
#include <algorithm>
#include <unordered_map>

#pragma once

inline const double EPSILON = 1e-6;

namespace renderer {

    struct Parameters {                 //All the parameters for drawing
        int width;
        int height;
        int padding;
        int stop_radius;
        int line_width;
        int bus_label_font_size;
        std::vector<int> bus_label_offset;
        int stop_label_font_size;
        std::vector<int> stop_label_offset;
        std::vector<double> underlayer_color;
        int underlayer_width;
    };

    bool IsZero(double);

    class SphereProjector {   //mapping points on the necessary map (set up by values padding, min_lon_ and max_lat_ in the class)
    public:
        template<typename PointInputIt>
        SphereProjector(PointInputIt points_begin, PointInputIt points_end,
                        double max_width, double max_height, double padding)
                : padding_(padding) {
            if (points_begin == points_end) {
                return;
            }

            const auto [left_it, right_it] = std::minmax_element(
                    points_begin, points_end,
                    [](auto lhs, auto rhs) { return lhs.lng < rhs.lng; });
            min_lon_ = left_it->lng;
            const double max_lon = right_it->lng;

            const auto [bottom_it, top_it] = std::minmax_element(
                    points_begin, points_end,
                    [](auto lhs, auto rhs) { return lhs.lat < rhs.lat; });
            const double min_lat = bottom_it->lat;
            max_lat_ = top_it->lat;

            std::optional<double> width_zoom;
            if (!IsZero(max_lon - min_lon_)) {
                width_zoom = (max_width - 2 * padding) / (max_lon - min_lon_);
            }

            std::optional<double> height_zoom;
            if (!IsZero(max_lat_ - min_lat)) {
                height_zoom = (max_height - 2 * padding) / (max_lat_ - min_lat);
            }

            if (width_zoom && height_zoom) {
                zoom_coeff_ = std::min(*width_zoom, *height_zoom);
            } else if (width_zoom) {
                zoom_coeff_ = *width_zoom;
            } else if (height_zoom) {
                zoom_coeff_ = *height_zoom;
            }
        }


        inline svg::Point operator()(Geographic::Coordinates coords) const {   //method for scaling coordinates, return scaled points
            return {
                    (coords.lng - min_lon_) * zoom_coeff_ + padding_,
                    (max_lat_ - coords.lat) * zoom_coeff_ + padding_
            };
        }

    private:
        double padding_;
        double min_lon_ = 0;
        double max_lat_ = 0;
        double zoom_coeff_ = 0;
    };


    class DrawRoute {        //DrawRoute using SVG library
    private:
        std::unordered_map<std::string, RouteImitation::Bus *> &routes;
        std::unordered_map<std::string, RouteImitation::Stop> &stops_coordinates;
        std::vector<Geographic::Coordinates> AllCords;
        Parameters params;
        svg::Document doc;  //field for drawing (pushing here will draw the picture)
    public:
        inline Parameters &GetParams() {   //getting parameters by reference
            return params;
        }

        DrawRoute() = default;

        DrawRoute(std::unordered_map<std::string, RouteImitation::Stop> &,
                  std::unordered_map<std::string, RouteImitation::Bus *> &route);

        void SetAll(std::unordered_map<std::string, RouteImitation::Stop> &,      //Set private fields in the class
                    std::unordered_map<std::string, RouteImitation::Bus *> &);

        void Draw(const std::string& file_name);  //Draw method, take name of output file

    };

}