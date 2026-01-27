#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "Vectors.h"
#include "MySfml.h"

namespace spiel
{
    class VisualDetail
    {
        void saveByte(std::ostream& out);
        void loadByte(std::istream& in);

        inline sf::Shape& getShape() { return *shape; }
        inline const sf::Shape& getShape() const { return *shape; }

        void setNone();
        sf::CircleShape& setCircle(float radius, int pointCount = 30);
        sf::ConvexShape& setLine(Vec2f from, Vec2f to, float width = 1);
        sf::RectangleShape& setRectangle(Vec2f angle1, Vec2f angle2);
        sf::RectangleShape& setRectangle(sf::Rect<float> rect);
        sf::ConvexShape& setPolygon(const std::vector<Vec2f>& points);
    private:
        std::unique_ptr<sf::Shape> shape;

        static constexpr int16_t shapeTypeNone = 0;
        static constexpr int16_t shapeTypeCircle = 1;
        static constexpr int16_t shapeTypeRectangle = 2;
        static constexpr int16_t shapeTypePolygon = 3;

        // struct ShapeData
        // {
        //     Vec2f position;
        //     Vec2f origin;
        //     float rotation{0};
        //     Vec2f scale{1, 1};
        //     uint32_t fillColor{static_cast<uint32_t>(sf::Color::Green.toInteger())};
        //     uint32_t outlineColor{0};
        //     float outlineThickness{0};
        // };

        // struct CircleData : public ShapeData
        // {
        //     float radius;
        //     uint32_t pointCount{30};
        // };
        // struct RectangleData : public ShapeData
        // {
        //     Vec2f size;
        // };
        // struct PolygonData : public ShapeData
        // {
        //     std::vector<Vec2f> points;
        // };
    };
} // namespace spiel
