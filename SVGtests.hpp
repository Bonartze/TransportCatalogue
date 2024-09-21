#define _USE_MATH_DEFINES

#include "svg.h"
#include <cassert>
#include <cmath>
#include <gtest/gtest.h>


//Just tests

namespace SVGtest {
    svg::Polyline CreateStar(svg::Point center, double outer_rad, double inner_rad, int num_rays);


    namespace shapes {
        class Triangle : public svg::Drawable {
        public:
            Triangle(svg::Point p1, svg::Point p2, svg::Point p3);

            void Draw(svg::ObjectContainer &container) const override;

        private:
            svg::Point p1_, p2_, p3_;
        };

        class Star : public svg::Drawable {
        public:
            Star(svg::Point p, double b_l, double i_r, int b_n);

            void Draw(svg::ObjectContainer &container) const override;

        private:
            svg::Point centre_;
            double beam_length;
            double inside_radios;
            int beam_number;
        };

        class Snowman : public svg::Drawable {
        public:
            Snowman(svg::Point c_, int r);

            void Draw(svg::ObjectContainer &container) const override;

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

    void RunAllTests();
}
