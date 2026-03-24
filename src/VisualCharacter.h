#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <any>
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>

#include "Vectors.h"
#include "JSON/json.h"
#include "JSON/json_builder.h"

namespace spiel
{
    class VisualShape
    {
    public:
        VisualShape() = default;
        ~VisualShape() = default;
        VisualShape(const VisualShape& other) = default;
        VisualShape(VisualShape&& other) noexcept = default;
        VisualShape& operator=(const VisualShape& other) = default;
        VisualShape& operator=(VisualShape&& other) noexcept = default;

        sf::ConvexShape getDrawable() const;
        json::Node toJson() const;

        VisualShape& byJson(const json::Node& node);
        VisualShape& setColor(const sf::Color& color);
        VisualShape& reserve(size_t size);
        VisualShape& setPointCount(size_t size);
        VisualShape& pushBack(const Vec2f& point);

        VisualShape& scale(const Vec2f& scale);
        VisualShape& scale(float scale);
        VisualShape& rotate(float radians);
        VisualShape& rotate(const Vec2f& complex);
        VisualShape& translate(const Vec2f& translation);

        VisualShape scaled(const Vec2f& scale) const;
        VisualShape scaled(float scale) const;
        VisualShape rotated(float radians) const;
        VisualShape rotated(const Vec2f& complex) const;
        VisualShape translated(const Vec2f& translation) const;

        const sf::Color& getColor() const;
        sf::Color& getColor();
        const Vec2f& getVertex(size_t index) const;
        Vec2f& getVertex(size_t index);
        size_t getVertexCount() const;
        VisualShape& setVertex(size_t index, const Vec2f& vertex);

        class Factory
        {
        public:
            virtual VisualShape create(const json::Node& node) = 0;
            virtual ~Factory() = default;
        };

    private:
        sf::Color color;
        std::vector<Vec2f> vertices;
    };

    class VisualCharacter
    {
    public:
    private:
        std::variant<VisualShape, std::vector<VisualCharacter>> data;
    };
}