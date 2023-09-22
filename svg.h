#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <iomanip>
#include <optional>
#include <variant>
#include <vector>

namespace svg {
    enum class StrokeLineCap {
        BUTT,
        ROUND,
        SQUARE,
    };

    enum class StrokeLineJoin {
        ARCS,
        BEVEL,
        MITER,
        MITER_CLIP,
        ROUND,
    };
    struct Rgb {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    struct Rgba {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        double opacity;
    };


    using Color = std::variant<std::monostate, std::string, Rgb, Rgba>;


     struct PrintColor {
        std::ostream &out;

        void operator()(std::monostate) const {
            out << "none";
        }

        void operator()(std::string &color) const {
            out << color;
        }

        void operator()(Rgb r) const {
            out << "rgb(" << std::to_string(r.red) << "," << std::to_string(r.green) << "," << std::to_string(r.blue)
                << ")";
        }

        void operator()(Rgba r) {
            out << "rgba(" << std::to_string(r.red) << "," << std::to_string(r.green) << "," << std::to_string(r.blue)
                << "," << r.opacity << ")";
        }
    };

    inline std::ostream &operator<<(std::ostream &out, Color color) {
        std::visit(PrintColor{out}, color);
        return out;
    }


    struct Point {
        Point() = default;

        Point(double x, double y)
                : x(x), y(y) {
        }

        double x = 0;
        double y = 0;
    };

    struct RenderContext {
        explicit RenderContext(std::ostream &out)
                : out(out) {
        }

        RenderContext(std::ostream &out, int indent_step, int indent = 0)
                : out(out), indent_step(indent_step), indent(indent) {
        }

        [[nodiscard]] RenderContext Indented() const {
            return {out, indent_step, indent + indent_step};
        }

        void RenderIndent() const {
            for (int i = 0; i < indent; ++i) {
                out.put(' ');
            }
        }

        std::ostream &out;
        int indent_step = 0;
        int indent = 0;
    };

    class Object {
    public:
        void Render(const RenderContext &context) const;

        virtual ~Object() = default;

    private:
        virtual void RenderObject(const RenderContext &context) const = 0;
    };

    std::ostream &operator<<(std::ostream &, const std::optional<StrokeLineCap> &);

    std::ostream &operator<<(std::ostream &, const std::optional<StrokeLineJoin> &);

    template<typename Owner>
    class PathProps {
    public:
        PathProps(const PathProps &other) : stroke_color_(other.stroke_color_), fill_color_(other.fill_color_),
                                            stroke_width_(other.stroke_width_), stroke_line_cap(other.stroke_line_cap),
                                            stroke_line_join(other.stroke_line_join) {}

        PathProps() = default;

        Owner &SetFillColor(Color color) {
            fill_color_ = color;
            return static_cast<Owner &>(*this);
        }

        Owner &SetStrokeColor(Color color) {
            this->stroke_color_ = std::move(color);
            return static_cast<Owner &>(*this);
        }

        Owner &SetStrokeWidth(double width) {
            stroke_width_ = width;
            return static_cast<Owner &>(*this);
        }

        Owner &SetStrokeLineCap(StrokeLineCap strokeLineCap) {
            stroke_line_cap = strokeLineCap;
            return static_cast<Owner &>(*this);
        }

        Owner &SetStrokeLineJoin(StrokeLineJoin strokeLineJoin) {
            stroke_line_join = strokeLineJoin;
            return static_cast<Owner &>(*this);
        }

        void RenderAttrs(std::ostream &out) const {
            using namespace std::literals;

            if (fill_color_.has_value()) {
                out << " fill=\"" << *fill_color_ << "\"";
            }
            if (stroke_color_) {
                out << " stroke=\"" << *stroke_color_ << "\"";
            }
            if (stroke_width_.has_value()) {
                out << " stroke-width=\"" << *stroke_width_ << "\"";
            }
            if (stroke_line_cap.has_value()) {
                out << stroke_line_cap;
            }
            if (stroke_line_join.has_value()) {
                out << stroke_line_join;
            }
        }


    protected:
        ~PathProps() = default;

        std::optional<Color> stroke_color_;
        std::optional<Color> fill_color_;
        std::optional<double> stroke_width_;
        std::optional<StrokeLineCap> stroke_line_cap;
        std::optional<StrokeLineJoin> stroke_line_join;
    private:
        Owner &AsOwner() {
            return static_cast<Owner &>(*this);
        }


    };

    class Circle final : public Object, public PathProps<Circle> {
    public:
        Circle() = default;

        Circle(const Circle &) = default;

        ~Circle() override = default;

        Circle &SetCenter(Point center);

        Circle &SetRadius(double radius);


    private:
        void RenderObject(const RenderContext &context) const override;

        Point center_;
        double radius_ = 1.0;
    };

    class Polyline final : public Object, public PathProps<Polyline> {
    public:
        Polyline() = default;

        Polyline(const Polyline &polyline) = default;

        ~Polyline() override = default;


        Polyline &AddPoint(Point point);


    private:
        std::vector<Point> points;

        void RenderObject(const RenderContext &context) const override;
    };


    class Text final : public Object, public PathProps<Text> {
    public:
        Text() = default;

        Text(const Text &) = default;

        ~Text() override = default;

        Text &SetPosition(Point pos);

        Text &SetOffset(Point offset);

        Text &SetFontSize(uint32_t size);

        Text &SetFontFamily(std::string font_family);

        Text &SetFontWeight(std::string font_weight);

        Text &SetData(std::string data);

        [[nodiscard]]std::string replaceSpecialCharacters() const;

    private:

        Point TextCoordinates = {0, 0};
        Point TextShifts = {0, 0};
        uint32_t FontSize = 1;
        std::string FontWeight;
        std::string FontFamily;
        std::string text;

        void RenderObject(const RenderContext &context) const override;
    };

    class ObjectContainer {
    public:

        virtual void AddPtr(std::unique_ptr<Object> &&) = 0;

        template<typename Obj>
        void Add(Obj obj) {
            DocumentFabric.emplace_back(std::make_unique<Obj>(std::move(obj)));
        }


    protected:
        std::vector<std::unique_ptr<Object>> DocumentFabric;

    };


    class Document : public ObjectContainer {
    public:
        Document() = default;


        void AddPtr(std::unique_ptr<Object> &&obj) override;

        void Render(std::ostream &out) const;


    };


    class Drawable {
    public:
        virtual void Draw(ObjectContainer &) const = 0;
    };
}