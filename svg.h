#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace svg {

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

    class Circle final : public Object {
    public:
        Circle() = default;

        ~Circle() override = default;

        Circle &SetCenter(Point center);

        Circle &SetRadius(double radius);


    private:
        void RenderObject(const RenderContext &context) const override;

        Point center_;
        double radius_ = 1.0;
    };

    class Polyline final : public Object {
    public:
        Polyline() = default;

        ~Polyline() override = default;


        // Добавляет очередную вершину к ломаной линии
        Polyline &AddPoint(Point point);


    private:
        std::vector<Point> points;

        void RenderObject(const RenderContext &context) const override;
    };


    class Text final : public Object {
    public:
        Text() = default;


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

        virtual void Add(Circle) = 0;

        virtual void Add(Polyline) = 0;

        virtual void Add(Text) = 0;

        virtual void AddPtr(std::unique_ptr<Object> &&) = 0;

    };


    class Document : public ObjectContainer {
    public:
        Document() = default;

        /*template<class Obj>
        void Add(Obj obj);
*/
        void Add(Circle) override;

        void Add(Polyline) override;

        void Add(Text) override;

        void AddPtr(std::unique_ptr<Object> &&obj) override;

        void Render(std::ostream &out) const;


    private:
        std::vector<std::unique_ptr<Object>> DocumentFabric;
    };


    class Drawable {
    public:
        virtual void Draw(ObjectContainer &) const= 0;
    };
}