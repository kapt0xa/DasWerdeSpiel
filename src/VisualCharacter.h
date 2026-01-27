#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>

#include "Vectors.h"
#include "MySfml.h"

namespace spiel
{
    class VisualDetail
    {
    public:
        void saveByte(std::ostream& out) const;
        void loadByte(std::istream& in);

        inline bool isNone() const { return shape == nullptr; }
        inline sf::Shape& getShape() { if(isNone()) throw std::runtime_error("VisualDetail::getShape: Shape is None"); return *shape; }
        inline const sf::Shape& getShape() const { if(isNone()) throw std::runtime_error("VisualDetail::getShape: Shape is None"); return *shape; }

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
    };
    class VisualCharacter
    {
    public:
        void saveByte(std::ostream& out);
        void loadByte(std::istream& in);

        inline VisualDetail& getDetail(size_t index) { return details.at(index); }
        inline const VisualDetail& getDetail(size_t index) const { return details.at(index); }

        inline size_t getDetailCount() const { return details.size(); }
        inline void setDetailCount(size_t count) { details.resize(count); }

        std::vector<size_t> findNoneDetails() const;
    private:
        std::vector<VisualDetail> details;
    };
} // namespace spiel
