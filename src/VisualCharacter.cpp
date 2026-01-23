#include "VisualCharacter.h"
#include <sstream>

namespace spiel
{
    namespace binarySaveLoad
    {
        struct FloatWrapper
        {
            float value;
            FloatWrapper(float&& v) : value(v) {}
        };
        struct FloatRefWrapper
        {
            float& value;
            FloatRefWrapper(float& v) : value(v) {}
        };
        std::ostream& operator<<(std::ostream& out, const sf::Vector2f& vec)
        {
            out.write(reinterpret_cast<const char*>(&vec.x), sizeof(vec.x));
            out.write(reinterpret_cast<const char*>(&vec.y), sizeof(vec.y));
            return out;
        }
        std::ostream& operator<<(std::ostream& out, const sf::Color& color)
        {
            uint32_t colorInt = color.toInteger();
            out.write(reinterpret_cast<const char*>(&colorInt), sizeof(colorInt));
            return out;
        }
        std::ostream& operator<<(std::ostream& out, FloatWrapper value)
        {
            out.write(reinterpret_cast<const char*>(&value.value), sizeof(value.value));
            return out;
        }
        std::ostream& operator<<(std::ostream& out, FloatRefWrapper value)
        {
            out.write(reinterpret_cast<const char*>(&value.value), sizeof(value.value));
            return out;
        }
        std::istream& operator>>(std::istream& in, sf::Vector2f& vec)
        {
            in.read(reinterpret_cast<char*>(&vec.x), sizeof(vec.x));
            in.read(reinterpret_cast<char*>(&vec.y), sizeof(vec.y));
            return in;
        }
        std::istream& operator>>(std::istream& in, sf::Color& color)
        {
            uint32_t colorInt;
            in.read(reinterpret_cast<char*>(&colorInt), sizeof(colorInt));
            color = sf::Color(colorInt);
            return in;
        }
        std::istream& operator>>(std::istream& in, FloatWrapper& value)
        {
            in.read(reinterpret_cast<char*>(&value.value), sizeof(value.value));
            return in;
        }
        std::istream& operator>>(std::istream& in, FloatRefWrapper& value)
        {
            in.read(reinterpret_cast<char*>(&value.value), sizeof(value.value));
            return in;
        }
    }
    void VisualDetail::saveByte(std::ostream& out)
    {
        uint16_t dataSize;
        if(!shape)
        {
            dataSize = 4;
            uint16_t shapeType = shapeTypeNone;
            out.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
            out.write(reinterpret_cast<const char*>(&shapeType), sizeof(shapeType));
            return;
        }

        if(auto circle = dynamic_cast<sf::CircleShape*>(shape.get()))
        {
            uint16_t shapeType = shapeTypeCircle;
            out.write(reinterpret_cast<const char*>(&shapeType), sizeof(shapeType));
            float radius = circle->getRadius();
            out.write(reinterpret_cast<const char*>(&radius), sizeof(radius));
            uint32_t pointCount = static_cast<uint32_t>(circle->getPointCount());
            out.write(reinterpret_cast<const char*>(&pointCount), sizeof(pointCount));
            return;
        }
        else if(auto rectangle = dynamic_cast<sf::RectangleShape*>(shape.get()))
        {
            uint16_t shapeType = shapeTypeRectangle;
            out.write(reinterpret_cast<const char*>(&shapeType), sizeof(shapeType));
            sf::Vector2f size = rectangle->getSize();
            out.write(reinterpret_cast<const char*>(&size), sizeof(size));
            return;
        }
        else if(auto polygon = dynamic_cast<sf::ConvexShape*>(shape.get()))
        {
            uint16_t shapeType = shapeTypePolygon;
            out.write(reinterpret_cast<const char*>(&shapeType), sizeof(shapeType));
            uint32_t pointCount = static_cast<uint32_t>(polygon->getPointCount());
            out.write(reinterpret_cast<const char*>(&pointCount), sizeof(pointCount));
            for(uint32_t i = 0; i < pointCount; ++i)
            {
                sf::Vector2f point = polygon->getPoint(i);
                out.write(reinterpret_cast<const char*>(&point), sizeof(point));
            }
            return;
        }
        // Unknown shape type
        throw std::runtime_error("VisualDetail::saveByte: Unknown shape type");
    }
    bool VisualDetail::loadByte(std::istream& in);

    void VisualDetail::setNone()
    {
        shape.reset();
    }

    sf::CircleShape& VisualDetail::setCircle(float radius, int pointCount = 30)
    {
        shape = std::make_unique<sf::CircleShape>(radius, pointCount);
        return static_cast<sf::CircleShape&>(*shape);
    }

    sf::ConvexShape& VisualDetail::setLine(Vec2f from, Vec2f to, float width = 1)
    {
        shape = std::make_unique<sf::ConvexShape>(Line(from, to, width));
        return static_cast<sf::ConvexShape&>(*shape);
    }

    sf::RectangleShape& VisualDetail::setRectangle(Vec2f angle1, Vec2f angle2)
    {
        shape = std::make_unique<sf::RectangleShape>(Rectangle(Cover(angle1, angle2)));
        return static_cast<sf::RectangleShape&>(*shape);
    }

    sf::RectangleShape& VisualDetail::setRectangle(sf::Rect<float> rect)
    {
        shape =  std::make_unique<sf::RectangleShape>(Rectangle(rect));
        return static_cast<sf::RectangleShape&>(*shape);
    }

    sf::ConvexShape& VisualDetail::setPolygon(const std::vector<Vec2f>& points)
    {
        shape = std::make_unique<sf::ConvexShape>(sf::ConvexShape(points.size()));
        for(size_t i = 0; i < points.size(); ++i)
        {
            static_cast<sf::ConvexShape&>(*shape).setPoint(i, vectorCast::toSFML(points[i]));
        }
        return static_cast<sf::ConvexShape&>(*shape);
    }

} // namespace spiel