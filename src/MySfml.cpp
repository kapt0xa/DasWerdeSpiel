#include "MySfml.h"
#include <array>
#include <functional>
#include <filesystem>

namespace spiel
{
    // 0..22 known type, -1 unknown
    ptrdiff_t getEventType(const sf::Event& event)
    {
        ptrdiff_t idx = -1;
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
    std::string getEventTypeName(const sf::Event& event)
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

    sf::ConvexShape Line(Vec2f from, Vec2f to, float width)
    {
        if(from == to)
        {
            return sf::ConvexShape();
        }
        sf::Vector2f direction = vectorCast::toSFML(normalized(to - from));
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

    sf::Rect<float> Cover(Vec2f a, Vec2f b)
    {
        float left = std::min(X(a), X(b));
        float top = std::min(Y(a), Y(b));
        float right = std::max(X(a), X(b));
        float bottom = std::max(Y(a), Y(b));
        return sf::Rect<float>({left, top}, {right - left, bottom - top});
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

    sf::Font& GetDefaultFont()
    {
        static sf::Font defaultFont(std::filesystem::path("../resrc/UbuntuSansMono[wght].ttf"));
        return defaultFont;
    }
}