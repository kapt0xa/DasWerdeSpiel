#include "MySfml.h"
#include <array>

namespace spiel
{
    // 0..22 known type, -1 unknown
    int getEventType(sf::Event& event)
    {
        int idx = -1;
        event.visit([&](const auto& sub) {
            using T = std::decay_t<decltype(sub)>;
            if constexpr (std::is_same_v<T, sf::Event::Closed>)                 idx = 0;
            else if constexpr (std::is_same_v<T, sf::Event::Resized>)           idx = 1;
            else if constexpr (std::is_same_v<T, sf::Event::FocusLost>)         idx = 2;
            else if constexpr (std::is_same_v<T, sf::Event::FocusGained>)       idx = 3;
            else if constexpr (std::is_same_v<T, sf::Event::TextEntered>)       idx = 4;
            else if constexpr (std::is_same_v<T, sf::Event::KeyPressed>)        idx = 5;
            else if constexpr (std::is_same_v<T, sf::Event::KeyReleased>)       idx = 6;
            else if constexpr (std::is_same_v<T, sf::Event::MouseWheelScrolled>)idx = 7;
            else if constexpr (std::is_same_v<T, sf::Event::MouseButtonPressed>)idx = 8;
            else if constexpr (std::is_same_v<T, sf::Event::MouseButtonReleased>)idx = 9;
            else if constexpr (std::is_same_v<T, sf::Event::MouseMoved>)        idx = 10;
            else if constexpr (std::is_same_v<T, sf::Event::MouseMovedRaw>)     idx = 11;
            else if constexpr (std::is_same_v<T, sf::Event::MouseEntered>)      idx = 12;
            else if constexpr (std::is_same_v<T, sf::Event::MouseLeft>)         idx = 13;
            else if constexpr (std::is_same_v<T, sf::Event::JoystickButtonPressed>)  idx = 14;
            else if constexpr (std::is_same_v<T, sf::Event::JoystickButtonReleased>) idx = 15;
            else if constexpr (std::is_same_v<T, sf::Event::JoystickMoved>)     idx = 16;
            else if constexpr (std::is_same_v<T, sf::Event::JoystickConnected>) idx = 17;
            else if constexpr (std::is_same_v<T, sf::Event::JoystickDisconnected>) idx = 18;
            else if constexpr (std::is_same_v<T, sf::Event::TouchBegan>)        idx = 19;
            else if constexpr (std::is_same_v<T, sf::Event::TouchMoved>)        idx = 20;
            else if constexpr (std::is_same_v<T, sf::Event::TouchEnded>)        idx = 21;
            else if constexpr (std::is_same_v<T, sf::Event::SensorChanged>)     idx = 22;
        });
        return idx;
    }

    std::array<std::string, 23> typeNames = {
        "Closed",
        "Resized",
        "FocusLost",
        "FocusGained",
        "TextEntered",
        "KeyPressed",
        "KeyReleased",
        "MouseWheelScrolled",
        "MouseButtonPressed",
        "MouseButtonReleased",
        "MouseMoved",
        "MouseMovedRaw",
        "MouseEntered",
        "MouseLeft",
        "JoystickButtonPressed",
        "JoystickButtonReleased",
        "JoystickMoved",
        "JoystickConnected",
        "JoystickDisconnected",
        "TouchBegan",
        "TouchMoved",
        "TouchEnded",
        "SensorChanged"
    };
    std::string getEventTypeName(sf::Event& event)
    {

        int idx = getEventType(event);
        if(idx != -1)
        {
            return typeNames[idx];
        }
        else
        {
            return "Unknown";
        }
    }

    float magnitude(Comp2f vec)
    {
        auto len2 = X(vec) * X(vec) + Y(vec) * Y(vec);
        if(len2 < floatMinNormal)
        {
            if(X(vec) == 0 && Y(vec) == 0)
            {
                return 0;
            }
            vec *= (1 / floatHalfSmall);
            len2 = X(vec) * X(vec) + Y(vec) * Y(vec);
            if(len2 < floatMinNormal)
            {
                vec *= (1 / floatRelativeEpsilon);
                len2 = X(vec) * X(vec) + Y(vec) * Y(vec);
                return std::sqrt(len2) * floatRelativeEpsilon * floatHalfSmall;
            }
            return std::sqrt(len2) * floatHalfSmall;
        }
        else if(std::isinf(len2))
        {
            vec *= floatHalfSmall;
            len2 = X(vec) * X(vec) + Y(vec) * Y(vec);
            return std::sqrt(len2) * (1 / floatHalfSmall);
        }
        return std::sqrt(len2);
    }

    Comp2f normalized(Comp2f vec, Comp2f defaultVec)
    {
        float length = magnitude(vec);

        if(length < floatMinNormal)
        {
            if(length == 0.0f)
            {
                return defaultVec;
            }
            vec *= (1 / floatHalfSmall);
            return vec / magnitude(vec);
        }
        auto result = vec / length;
        return result;
    }

    sf::ConvexShape Line(Comp2f from, Comp2f to, float width)
    {
        if(from == to)
        {
            return sf::ConvexShape();
        }
        sf::Vector2f direction = Cast<>::ToSF(to - from).normalized();
        sf::Vector2f normal = {-direction.y, direction.x};

        sf::ConvexShape line;
        line.setPointCount(4);
        line.setPoint(0, {X(from) + normal.x * width / 2, Y(from) + normal.y * width / 2});
        line.setPoint(1, {X(from) - normal.x * width / 2, Y(from) - normal.y * width / 2});
        line.setPoint(2, {X(to) - normal.x * width / 2, Y(to) - normal.y * width / 2});
        line.setPoint(3, {X(to) + normal.x * width / 2, Y(to) + normal.y * width / 2});
        return line;
    }

    sf::CircleShape Circle(float radius, int pointCount)
    {
        sf::CircleShape circle(radius, pointCount);
        circle.setOrigin({radius, radius});
        return circle;
    }

    sf::Rect<float> Cover(sf::Rect<float> a, sf::Rect<float> b)
    {
        float left = std::min(a.position.x, b.position.x);
        float top = std::min(a.position.y, b.position.y);
        float right = std::max(a.position.x + a.size.x, b.position.x + b.size.x);
        float bottom = std::max(a.position.y + a.size.y, b.position.y + b.size.y);
        return sf::Rect<float>({left, top}, {right - left, bottom - top});
    }

    sf::RectangleShape Rectangle(sf::Rect<float> rect)
    {
        sf::RectangleShape rectangle(rect.size);
        rectangle.setOrigin(rect.size * 0.5f);
        rectangle.setPosition(rect.position + rect.size * 0.5f);
        return rectangle;
    }
}