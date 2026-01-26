#include "VisualCharacter.h"
#include <sstream>
#include <climits>
#include <cassert>

namespace spiel
{
    namespace binarySaveLoad
    {
        template <typename T>
        struct TWrapper
        {
            T value;
            TWrapper(T&& v) : value(v) {}
        };
        template <typename T>
        struct TRefWrapper
        {
            T& value;
            TRefWrapper(T& v) : value(v) {}
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
        template <typename T>
        std::ostream& operator<<(std::ostream& out, const TWrapper<T>& value)
        {
            out.write(reinterpret_cast<const char*>(&value.value), sizeof(value.value));
            return out;
        }
        template <typename T>
        std::ostream& operator<<(std::ostream& out, TRefWrapper<T>&& value)
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
        template <typename T>
        std::istream& operator>>(std::istream& in, TWrapper<T>& value)
        {
            in.read(reinterpret_cast<char*>(&value.value), sizeof(value.value));
            return in;
        }
        template <typename T>
        std::istream& operator>>(std::istream& in, TRefWrapper<T>&& value)
        {
            in.read(reinterpret_cast<char*>(&value.value), sizeof(value.value));
            return in;
        }
    }
    void saveByteShape(std::ostream& out, sf::Shape& shape)
    {
        using namespace binarySaveLoad;
        // Color          m_fillColor{Color::White};    //!< Fill color
        // Color          m_outlineColor{Color::White}; //!< Outline color
        // float          m_outlineThickness{};         //!< Thickness of the shape's outline
        // Vector2f          m_origin;                           //!< Origin of translation/rotation/scaling of the object
        // Vector2f          m_position;                         //!< Position of the object in the 2D world
        // Angle             m_rotation;                         //!< Orientation of the object
        // Vector2f          m_scale{1, 1}; 

        out << shape.getFillColor()
            << shape.getOutlineColor()
            << TWrapper(shape.getOutlineThickness())
            << shape.getOrigin()
            << shape.getPosition()
            << TWrapper(shape.getRotation().asRadians())
            << shape.getScale();
    }
    void loadByteShape(std::istream& in, sf::Shape& shape)
    {
        using namespace binarySaveLoad;

        sf::Color fillColor;
        sf::Color outlineColor;
        float outlineThickness;
        sf::Vector2f origin;
        sf::Vector2f position;
        float rotationRadians;
        sf::Vector2f scale;

        in >> fillColor
           >> outlineColor
           >> TRefWrapper(outlineThickness)
           >> origin
           >> position
           >> TRefWrapper(rotationRadians)
           >> scale;

        shape.setFillColor(fillColor);
        shape.setOutlineColor(outlineColor);
        shape.setOutlineThickness(outlineThickness);
        shape.setOrigin(origin);
        shape.setPosition(position);
        shape.setRotation(sf::radians(rotationRadians));
        shape.setScale(scale);
    }
    void VisualDetail::saveByte(std::ostream& out)
    {
        using namespace binarySaveLoad;

        uint16_t dataSize;
        uint16_t shapeType;
        std::stringstream buf;

        if(!shape)
        {
            dataSize = static_cast<uint16_t>(sizeof(shapeType) + sizeof(dataSize));
            shapeType = shapeTypeNone;
        }
        else if(auto circle = dynamic_cast<sf::CircleShape*>(shape.get()))
        {
            shapeType = shapeTypeCircle;
            buf << TWrapper(circle->getRadius())
                << TWrapper(static_cast<uint32_t>(circle->getPointCount()));
            saveByteShape(buf, *circle);
        }
        else if(auto rectangle = dynamic_cast<sf::RectangleShape*>(shape.get()))
        {
            shapeType = shapeTypeRectangle;
            buf << TWrapper(rectangle->getSize());
            saveByteShape(buf, *rectangle);
        }
        else if(auto polygon = dynamic_cast<sf::ConvexShape*>(shape.get()))
        {
            shapeType = shapeTypePolygon;
            uint32_t pointCount = static_cast<uint32_t>(polygon->getPointCount());
            buf << TRefWrapper(pointCount);
            assert(pointCount <= UINT32_MAX);
            for(uint32_t i = 0; i < pointCount; ++i)
            {
                buf << polygon->getPoint(i);
            }
            saveByteShape(buf, *polygon);
        }
        else
        {
            // Unknown shape type
            throw std::runtime_error("VisualDetail::saveByte: Unknown shape type");
        }
        std::string bufStr = buf.str();
        dataSize = static_cast<uint16_t>(bufStr.size() + sizeof(shapeType) + sizeof(dataSize));
        assert(bufStr.size() < static_cast<size_t>(dataSize));
        out << TRefWrapper(dataSize)
            << TRefWrapper(shapeType)
            << bufStr;
    }
    bool VisualDetail::loadByte(std::istream& in)
    {
        check this copilot written
        // using namespace binarySaveLoad;

        // uint16_t dataSize;
        // uint16_t shapeType;

        // in >> TRefWrapper(dataSize)
        //    >> TRefWrapper(shapeType);

        // switch(shapeType)
        // {
        //     case shapeTypeNone:
        //     {
        //         setNone();
        //         break;
        //     }
        //     case shapeTypeCircle:
        //     {
        //         float radius;
        //         uint32_t pointCount;
        //         in >> TRefWrapper(radius)
        //            >> TRefWrapper(pointCount);
        //         auto& circle = setCircle(radius, static_cast<int>(pointCount));
        //         loadByteShape(in, circle);
        //         break;
        //     }
        //     case shapeTypeRectangle:
        //     {
        //         sf::Vector2f size;
        //         in >> TRefWrapper(size);
        //         auto& rectangle = setRectangle(sf::Rect<float>({0, 0}, size));
        //         loadByteShape(in, rectangle);
        //         break;
        //     }
        //     case shapeTypePolygon:
        //     {
        //         uint32_t pointCount;
        //         in >> TRefWrapper(pointCount);
        //         std::vector<Vec2f> points(pointCount);
        //         for(uint32_t i = 0; i < pointCount; ++i)
        //         {
        //             in >> points[i];
        //         }
        //         auto& polygon = setPolygon(points);
        //         loadByteShape(in, polygon);
        //         break;
        //     }
        //     default:
        //     {
        //         // Unknown shape type
        //         throw std::runtime_error("VisualDetail::loadByte: Unknown shape type");
        //     }
        // }
        // return true;
    }

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