#include "VisualCharacter.h"

namespace spiel
{
    sf::ConvexShape VisualShape::getDrawable() const
    {
        sf::ConvexShape result;
        result.setFillColor(color);
        result.setPointCount(vertices.size());
        for (size_t i = 0; i < vertices.size(); ++i)
            result.setPoint(i, vectorCast::toSFML(vertices[i]));
        return result;
    }
    json::Dict colorToJson(const sf::Color& color)
    {
        return json::Builder().StartDict()
            .Key("r").Value(color.r)
            .Key("g").Value(color.g)
            .Key("b").Value(color.b)
            .Key("a").Value(color.a)
            .EndDict().Build().AsDict();
    }
    sf::Color colorByJson(const json::Node& node)
    {
        const auto& dict = node.AsDict();
        return sf::Color(
            dict.at("r").AsInt(),
            dict.at("g").AsInt(),
            dict.at("b").AsInt(),
            dict.at("a").AsInt()
        );
    }
    json::Dict vect2dToJson(const Vec2f& vec)
    {
        return json::Builder().StartDict()
            .Key("x").Value(X(vec))
            .Key("y").Value(Y(vec))
            .EndDict().Build().AsDict();
    }
    Vec2f vect2dByJson(const json::Node& node)
    {
        const auto& dict = node.AsDict();
        return Vec2f(static_cast<float>(dict.at("x").AsDouble()), static_cast<float>(dict.at("y").AsDouble()));
    }

    json::Node VisualShape::toJson() const
    {
        json::Array verticesArr;
        for (const auto& vertex : vertices)
        {
            verticesArr.push_back(vect2dToJson(vertex));
        }
        return json::Builder().StartDict()
            .Key("color").Value(colorToJson(color))
            .Key("vertices").Value(verticesArr)
            .EndDict().Build();
    }

    VisualShape& VisualShape::byJson(const json::Node& node)
    {
        const auto& dict = node.AsDict();
        color = colorByJson(dict.at("color"));
        const auto& verticesArr = dict.at("vertices").AsArray();
        setPointCount(0);
        reserve(verticesArr.size());
        for (const auto& vertexNode : verticesArr)
        {
            pushBack(vect2dByJson(vertexNode));
        }
        return *this;
    }
    VisualShape& VisualShape::setColor(const sf::Color& color)
    {
        this->color = color;
        return *this;
    }
    VisualShape& VisualShape::reserve(size_t size)
    {
        vertices.reserve(size);
        return *this;
    }
    VisualShape& VisualShape::setPointCount(size_t size)
    {
        vertices.resize(size);
        return *this;
    }
    VisualShape& VisualShape::pushBack(const Vec2f& point)
    {
        vertices.push_back(point);
        return *this;
    }

    VisualShape& VisualShape::scale(const Vec2f& scale)
    {
        for (auto& vertex : vertices)
        {
            X(vertex) *= X(scale);
            Y(vertex) *= Y(scale);
        }
        return *this;
    }
    VisualShape& VisualShape::scale(float scale)
    {
        return this->scale(Vec2f(scale, scale));
    }
    VisualShape& VisualShape::rotate(float radians)
    {
        Vec2f complex(std::cos(radians), std::sin(radians));
        return this->rotate(complex);
    }
    VisualShape& VisualShape::rotate(const Vec2f& complex)
    {
        for (auto& vertex : vertices)
        {
            vertex *= complex;
        }
        return *this;
    }
    VisualShape& VisualShape::translate(const Vec2f& translation)
    {
        for (auto& vertex : vertices)
        {
            vertex += translation;
        }
        return *this;
    }
    VisualShape VisualShape::scaled(const Vec2f& scale) const
    {
        VisualShape result = *this;
        result.scale(scale);
        return result;
    }
    VisualShape VisualShape::scaled(float scale) const
    {
        return this->scaled(Vec2f(scale, scale));
    }
    VisualShape VisualShape::rotated(float radians) const
    {
        VisualShape result = *this;
        result.rotate(radians);
        return result;
    }
    VisualShape VisualShape::rotated(const Vec2f& complex) const
    {
        VisualShape result = *this;
        result.rotate(complex);
        return result;
    }
    VisualShape VisualShape::translated(const Vec2f& translation) const
    {
        VisualShape result = *this;
        result.translate(translation);
        return result;
    }
    const sf::Color& VisualShape::getColor() const
    {
        return color;
    }
    sf::Color& VisualShape::getColor()
    {
        return color;
    }
    const Vec2f& VisualShape::getVertex(size_t index) const
    {
        return vertices.at(index);
    }
    Vec2f& VisualShape::getVertex(size_t index)
    {
        return vertices.at(index);
    }
    size_t VisualShape::getVertexCount() const
    {
        return vertices.size();
    }
    VisualShape& VisualShape::setVertex(size_t index, const Vec2f& vertex)
    {
        vertices.at(index) = vertex;
        return *this;
    }
}