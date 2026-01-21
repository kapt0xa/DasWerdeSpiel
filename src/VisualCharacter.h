#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "MySfml.h"

namespace spiel
{
    class VisualDetail
    {
        void saveByte(std::ostream& out);
        bool loadByte(std::istream& in);

        inline sf::Shape& getShape() { return *shape; }
        inline const sf::Shape& getShape() const { return *shape; }

        void setNone();
        sf::CircleShape& setCircle(float radius, int pointCount = 30);
        sf::ConvexShape& setLine(Comp2f from, Comp2f to, float width = 1);
        sf::RectangleShape& setRectangle(Comp2f angle1, Comp2f angle2);
        sf::RectangleShape& setRectangle(sf::Rect<float> rect);
        sf::ConvexShape& setPolygon(const std::vector<Comp2f>& points);
    private:
        std::unique_ptr<sf::Shape> shape;

        static constexpr uint16_t shapeTypeNone = 0;
        static constexpr uint16_t shapeTypeCircle = 1;
        static constexpr uint16_t shapeTypeRectangle = 2;
        static constexpr uint16_t shapeTypePolygon = 3;

        struct CircleData
        {
            float radius;
            float rotation;
            uint32_t pointCount;
            Comp2f position;
            Comp2f origin;
            sf::Color fillColor;
        };
        struct RectangleData
        {
            float 
        };
        
    };
} // namespace spiel
