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
            container.Add(CreateStar(centre_, beam_length, inside_radios, beam_number));
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
            container.Add(svg::Circle().SetCenter({centre_.x, centre_.y + 5 * radios}).SetRadius(2 * radios));
            container.Add(svg::Circle().SetCenter({centre_.x, centre_.y + 2 * radios}).SetRadius(1.5 * radios));
            container.Add(svg::Circle().SetCenter(centre_).SetRadius(radios));
        }

    private:
        svg::Point centre_;
        int radios;
    };

} // namespace shapes



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
    using namespace shapes;
    using namespace std;

    vector<unique_ptr<svg::Drawable>> picture;
    picture.emplace_back(make_unique<Triangle>(Point{100, 20}, Point{120, 50}, Point{80, 40}));
    picture.emplace_back(make_unique<Star>(Point{50.0, 20.0}, 10.0, 4.0, 5));
    picture.emplace_back(make_unique<Snowman>(Point{30, 20}, 10.0));

    svg::Document doc;
    DrawPicture(picture, doc);

    const Text base_text =  //
            Text()
                    .SetFontFamily("Verdana"s)
                    .SetFontSize(12)
                    .SetPosition({10, 100})
                    .SetData("Happy New Year!"s);
    doc.Add(Text{base_text}
                    .SetStrokeColor("yellow"s)
                    .SetFillColor("yellow"s)
                    .SetStrokeLineJoin(StrokeLineJoin::ROUND)
                    .SetStrokeLineCap(StrokeLineCap::ROUND)
                    .SetStrokeWidth(3));
    doc.Add(Text{base_text}.SetFillColor("red"s));

    doc.Render(cout);
}