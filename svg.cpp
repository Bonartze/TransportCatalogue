#include "svg.h"

namespace svg {

    using namespace std::literals;

    void Object::Render(const RenderContext &context) const {
        context.RenderIndent();

        RenderObject(context);

        context.out << std::endl;
    }

    std::ostream &operator<<(std::ostream &os, const std::optional<StrokeLineCap> &stroke_line_cap) {
        std::string cap_string;
        switch (*stroke_line_cap) {
            case StrokeLineCap::BUTT:
                cap_string = "butt";
                break;
            case StrokeLineCap::ROUND:
                cap_string = "round";
                break;
            case StrokeLineCap::SQUARE:
                cap_string = "square";
        }
        os << " stroke-linecap=\"" << cap_string << "\"";
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const std::optional<StrokeLineJoin> &stroke_line_join) {
        std::string join_string;
        switch (*stroke_line_join) {
            case StrokeLineJoin::ARCS:
                join_string = "arcs";
                break;
            case StrokeLineJoin::BEVEL:
                join_string = "bevel";
                break;
            case StrokeLineJoin::MITER:
                join_string = "miter";
                break;
            case StrokeLineJoin::MITER_CLIP:
                join_string = "miter-clip";
                break;
            case StrokeLineJoin::ROUND:
                join_string = "round";
        }
        os << " stroke-linejoin=\"" << join_string << "\"";
        return os;
    }


// ---------- Circle ------------------


    Circle &Circle::SetCenter(Point center) {
        center_ = center;
        return *this;
    }

    Circle &Circle::SetRadius(double radius) {
        radius_ = radius;
        return *this;
    }


    void Circle::RenderObject(const RenderContext &context) const {
        auto &out = context.out;
        out << "  <circle cx=\"" << center_.x << "\" cy=\"" << center_.y << "\" "s;
        out << "r=\"" << radius_ << "\" "s;
        RenderAttrs(context.out);
        out << "/>"s;
    }

// ---------- Polyline ------------------

    Polyline &Polyline::AddPoint(svg::Point point) {
        points.emplace_back(point);
        return *this;
    }

    void Polyline::RenderObject(const RenderContext &context) const {
        auto &out = context.out;
        out << "  <polyline points=\"";
        for (size_t i = 0; i < points.size(); ++i) {
            out << points[i].x << "," << points[i].y;
            if (i < points.size() - 1) {
                out << " ";
            }
        }
        out << "\"";
        RenderAttrs(context.out);
        out << "/>";
    }
// ---------- Text ------------------


    std::string Text::replaceSpecialCharacters() const {
        std::string result;

        for (size_t i = 0; i < text.length(); ++i) {
            if (text[i] == '<' && (i == 0 || text[i - 1] != '&')) {
                result += "&lt;";
            } else if (text[i] == '>' && (i == text.length() - 1 || text[i + 1] != ';')) {
                result += "&gt;";
            } else if (text[i] == '"' && (i == 0 || text[i - 1] != '&')) {
                result += "&quot;";
            } else if (text[i] == '\'' && (i == 0 || text[i - 1] != '&')) {
                result += "&apos;";
            } else if (text[i] == '&' && (i == 0 || text[i - 1] != '&')) {
                result += "&amp;";
            } else {
                result += text[i];
            }
        }
        return result;
    }

    void Text::RenderObject(const svg::RenderContext &context) const {
        auto &out = context.out;
        auto fText = replaceSpecialCharacters();
        out << "  <text ";
        RenderAttrs(context.out);
        out << " x=\"" << TextCoordinates.x << "\" " << "y=\"" << TextCoordinates.y << "\"" << " dx=\""
            << TextShifts.x
            << "\" dy=\"" << TextShifts.y << "\" font-size=\"" << FontSize << "\" font-family=\"" << FontFamily
            << "\" font-weight=\"" << FontWeight << "\">" << fText << " </text>";
    }

    Text &Text::SetPosition(svg::Point pos) {
        TextCoordinates = pos;

        return *this;
    }

    Text &Text::SetOffset(svg::Point offset) {
        TextShifts = offset;
        return *this;
    }

    Text &Text::SetFontSize(uint32_t size) {
        FontSize = size;
        return *this;
    }

    Text &Text::SetFontWeight(std::string font_weight) {
        FontWeight = std::move(font_weight);
        return *this;
    }

    Text &Text::SetFontFamily(std::string font_family) {
        FontFamily = std::move(font_family);
        return *this;
    }

    Text &Text::SetData(std::string data) {
        text = std::move(data);
        return *this;
    }

// ---------- Document ------------------


    void Document::AddPtr(std::unique_ptr<Object> &&obj) {
        DocumentFabric.emplace_back(std::move(obj));
    }


    void Document::Render(std::ostream &out) const {
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
               "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"
            << "  <rect width=\"100%\" height=\"100%\" fill=\"white\" />\n";
        for (auto &object: DocumentFabric)
            object->Render(RenderContext(out));
        out << "</svg>";
    }
}