#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <any>
#include <vector>
#include <string>
#include <unordered_map>

#include "Vectors.h"
#include "JSON/json.h"
#include "JSON/json_builder.h"

namespace spiel
{
    class Shape
    {
    public:
        Shape() = default;
        ~Shape() = default;
        Shape(const Shape& other) = default;
        Shape(Shape&& other) noexcept = default;
        Shape& operator=(const Shape& other) = default;
        Shape& operator=(Shape&& other) noexcept = default;

        sf::ConvexShape getDrawable() const;
        json::Node toJson() const;

        Shape& setColor(const sf::Color& color);
        Shape& reserve(size_t size);
        Shape& setPointCount(size_t size);
        Shape& pushBack(const Vec2f& point);

        Shape& scale(const Vec2f& scale);
        Shape& scale(float scale);
        Shape& rotate(float radians);
        Shape& rotate(const Vec2f& complex);

    private:
        sf::Color color;
        std::vector<Vec2f> vertices;
    };
}