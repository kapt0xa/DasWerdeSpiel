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
        VisualShape& resizePointArr(size_t size);
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
        VisualCharacter() = default;
        ~VisualCharacter() = default;
        VisualCharacter(const VisualCharacter& other) = default;
        VisualCharacter(VisualCharacter&& other) noexcept = default;
        VisualCharacter& operator=(const VisualCharacter& other) = default;
        VisualCharacter& operator=(VisualCharacter&& other) noexcept = default;

        json::Node toJson() const;

        VisualCharacter& byJson(const json::Node& node);
        VisualCharacter& setShape(const VisualShape& shape);
        VisualCharacter& setShape(VisualShape&& shape);
        VisualCharacter& setChildren(const std::vector<VisualCharacter>& children);

        VisualCharacter& setName(const std::string& name);
        const std::string& getName() const;

        operator bool();
        bool isNull() const;
        bool isShape() const;
        bool isGroup() const;
        const VisualShape& getShape() const;
        VisualShape& getShape();
        const VisualCharacter& getChild(size_t index) const;
        VisualCharacter& getChild(size_t index);
        VisualCharacter& setChild(size_t index, const VisualCharacter& value);
        VisualCharacter& setChild(size_t index, VisualCharacter&& value);
        VisualCharacter& getChild(size_t index);
        size_t getChildCount() const;
        VisualCharacter& resizeChildren(size_t size);
        VisualCharacter& pushBackChild(const VisualCharacter& child);
        VisualCharacter& reserveChildren(size_t size);

        VisualCharacter& setColor(const sf::Color& color);
        VisualCharacter& scale(const Vec2f& scale);
        VisualCharacter& scale(float scale);
        VisualCharacter& rotate(float radians);
        VisualCharacter& rotate(const Vec2f& complex);
        VisualCharacter& translate(const Vec2f& translation);
    private:
        std::string name;
        std::variant<std::monostate, VisualShape, std::vector<VisualCharacter>> data;
    };
}