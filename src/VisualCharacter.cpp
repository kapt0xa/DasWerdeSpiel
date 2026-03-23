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