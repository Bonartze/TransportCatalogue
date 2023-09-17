#define _USE_MATH_DEFINES

#include "svg.h"
#include <cmath>

namespace {
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
}


namespace shapes {

    class Triangle : public svg::Drawable {
    public:
        Triangle(svg::Point p1, svg::Point p2, svg::Point p3)
                : p1_(p1), p2_(p2), p3_(p3) {
        }

        void Draw(svg::ObjectContainer &container) const override {
            container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
        }

    private:
        svg::Point p1_, p2_, p3_;
    };

    class Star : public svg::Drawable {
    public:
        Star(svg::Point p, double b_l, double i_r, int b_n) : centre_(p),
                                                              beam_length(b_l), inside_radios(i_r), beam_number(b_n) {};

        void Draw(svg::ObjectContainer &container) const override {
            container.Add(
                    CreateStar(centre_, beam_length, inside_radios, beam_number).SetFillColor("red").SetStrokeColor(
                            "black"));
        }

    private:
        svg::Point centre_;
        double beam_length;
        double inside_radios;
        int beam_number;
    };

    class Snowman : public svg::Drawable {
    public:
        Snowman(svg::Point c_, int r) : centre_(c_), radios(r) {}

        void Draw(svg::ObjectContainer &container) const override {
            container.Add(
                    svg::Circle().SetCenter({centre_.x, centre_.y + 5 * radios}).SetRadius(2 * radios).SetFillColor(
                            "rgb(240,240,240)").SetStrokeColor("black"));
            container.Add(
                    svg::Circle().SetCenter({centre_.x, centre_.y + 2 * radios}).SetRadius(1.5 * radios).SetFillColor(
                            "rgb(240,240,240)").SetStrokeColor("black"));
            container.Add(svg::Circle().SetCenter(centre_).SetRadius(radios).SetFillColor(
                    "rgb(240,240,240)").SetStrokeColor("black"));
        }

    private:
        svg::Point centre_;
        int radios;
    };

}


template<typename DrawableIterator>
void DrawPicture(DrawableIterator begin, DrawableIterator end, svg::ObjectContainer &target) {
    for (auto it = begin; it != end; ++it) {
        (*it)->Draw(target);
    }
}

template<typename Container>
void DrawPicture(const Container &container, svg::ObjectContainer &target) {
    using namespace std;
    DrawPicture(begin(container), end(container), target);
}

int main() {
    using namespace svg;
    using namespace std;

    Color none_color;
    cout << none_color << endl; // none

    Color purple{"purple"s};
    cout << purple << endl; // purple

    Color rgb = Rgb{100, 200, 255};
    cout << rgb << endl; // rgb(100,200,255)

    Color rgba = Rgba{100, 200, 255, 0.5};
    cout << rgba << endl; // rgba(100,200,255,0.5)

    Circle c;
    c.SetRadius(3.5).SetCenter({1.0, 2.0});
    c.SetFillColor(rgba);
    c.SetStrokeColor(purple);

    Document doc;
    doc.Add(std::move(c));
    doc.Render(cout);
}
