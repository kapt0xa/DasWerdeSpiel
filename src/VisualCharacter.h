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
    struct ShapeTransform
    {
        Vec2f pos;
        float rot;
        float scale;
        sf::Color color;

        ShapeTransform add(const ShapeTransform& parent) const;
    };

    struct ShapeData : public ShapeTransform
    {
        virtual ~ShapeData() = default;

        virtual std::unique_ptr<sf::Drawable> buildDrawable() const = 0;
        virtual json::Node toJSON() const = 0;

        class JSONparser
        {
        public:
            virtual ~JSONparser() = default;
            virtual std::unique_ptr<ShapeData> operator()(const json::Node& node) = 0;
        };

        static const std::unordered_map<json::string, std::unique_ptr<JSONparser>> jsonParsers;
    };
}