#include "VisualCharacter.h"

namespace spiel
{
        sf::ConvexShape Shape::getDrawable() const
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
        sf::Color colorFromJson(const json::Node& node)
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
        Vec2f vect2dFromJson(const json::Node& node)
        {
            const auto& dict = node.AsDict();
            return Vec2f(static_cast<float>(dict.at("x").AsDouble()), static_cast<float>(dict.at("y").AsDouble()));
        }

        json::Node Shape::toJson() const
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

        Shape& byJson(const json::Node& node);
        Shape& Shape::setColor(const sf::Color& color)
        {
            this->color = color;
            return *this;
        }
        Shape& Shape::reserve(size_t size)
        {
            vertices.reserve(size);
            return *this;
        }
        Shape& Shape::setPointCount(size_t size)
        {
            vertices.resize(size);
            return *this;
        }
        Shape& Shape::pushBack(const Vec2f& point)
        {
            vertices.push_back(point);
            return *this;
        }

        Shape& Shape::scale(const Vec2f& scale)
        {
            for (auto& vertex : vertices)
            {
                X(vertex) *= X(scale);
                Y(vertex) *= Y(scale);
            }
            return *this;
        }
        Shape& Shape::scale(float scale)
        {
            return this->scale(Vec2f(scale, scale));
        }
        Shape& Shape::rotate(float radians)
        {
            Vec2f complex(std::cos(radians), std::sin(radians));
            return this->rotate(complex);
        }
        Shape& Shape::rotate(const Vec2f& complex)
        {
            for (auto& vertex : vertices)
            {
                vertex *= complex;
            }
            return *this;
        }
}