#define _USE_MATH_DEFINES

#include "../Mapping_src/svg.h"
#include <cmath>
#include "SVGtests.hpp"

#include "gtest/gtest.h"

//Just tests

namespace SVGtest {
    svg::Polyline CreateStar(svg::Point center, double outer_rad, double inner_rad, int num_rays) {
        using namespace svg;
        Polyline polyline;
        for (int i = 0; i <= num_rays; ++i) {
            double angle = 2 * M_PI * (i % num_rays) / num_rays;
            polyline.AddPoint({center.x + outer_rad * sin(angle), center.y - outer_rad * cos(angle)});
            if (i == num_rays) {
                break;
            }
            angle += M_PI / num_rays;
            polyline.AddPoint({center.x + inner_rad * sin(angle), center.y - inner_rad * cos(angle)});
        }
        return polyline;
    }

    namespace shapes {
        Triangle::Triangle(svg::Point p1, svg::Point p2, svg::Point p3)
            : p1_(p1), p2_(p2), p3_(p3) {
        }

        void Triangle::Draw(svg::ObjectContainer &container) const {
            container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
        }


        Star::Star(svg::Point p, double b_l, double i_r, int b_n) : centre_(p),
                                                                    beam_length(b_l), inside_radios(i_r),
                                                                    beam_number(b_n) {
        };

        void Star::Draw(svg::ObjectContainer &container) const {
            container.Add(
                CreateStar(centre_, beam_length, inside_radios, beam_number).SetFillColor("red").SetStrokeColor(
                    "black"));
        }


        Snowman::Snowman(svg::Point c_, int r) : centre_(c_), radios(r) {
        }

        void Snowman::Draw(svg::ObjectContainer &container) const {
            container.Add(
                svg::Circle().SetCenter({centre_.x, centre_.y + 5 * radios}).SetRadius(2 * radios).SetFillColor(
                    "rgb(240,240,240)").SetStrokeColor("black"));
            container.Add(
                svg::Circle().SetCenter({centre_.x, centre_.y + 2 * radios}).SetRadius(1.5 * radios).SetFillColor(
                    "rgb(240,240,240)").SetStrokeColor("black"));
            container.Add(svg::Circle().SetCenter(centre_).SetRadius(radios).SetFillColor(
                "rgb(240,240,240)").SetStrokeColor("black"));
        }
    }

    TEST(SVG_TEST, ALL_RUN) {
        std::cerr << "SVG tests OK\n";
        std::stringstream ss;
        using namespace svg;
        using namespace std;

        Color none_color;
        ss << none_color << endl;

        Color purple{"purple"s};
        ss << purple << endl;

        Color rgb = Rgb{100, 200, 255};
        ss << rgb << endl;

        Color rgba = Rgba{100, 200, 255, 0.5};
        ss << rgba << endl;

        Circle c;
        c.SetRadius(3.5).SetCenter({1.0, 2.0});
        c.SetFillColor(rgba);
        c.SetStrokeColor(purple);

        Document doc;
        doc.Add(std::move(c));
        doc.Render(ss);
        assert(ss.str() == "none\n"
            "purple\n"
            "rgb(100,200,255)\n"
            "rgba(100,200,255,0.5)\n"
            "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
            "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"
            "  <rect width=\"100%\" height=\"100%\" fill=\"white\" />\n"
            "  <circle cx=\"1\" cy=\"2\" r=\"3.5\"  fill=\"rgba(100,200,255,0.5)\" stroke=\"purple\"/>\n"
            "</svg>");
    }
}
