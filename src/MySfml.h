#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

#include "Vectors.h"

namespace spiel
{
    using SimpleEvent = std::function<void(void)>;
    using TickEvent = std::function<void(float deltaTime)>;

    ptrdiff_t getEventType(const sf::Event& event);
    std::string getEventTypeName(const sf::Event& event);

    template<ptrdiff_t Index>
    using EventType =
        std::conditional_t<Index == 0, sf::Event::Closed,
        std::conditional_t<Index == 1, sf::Event::Resized,
        std::conditional_t<Index == 2, sf::Event::FocusLost,
        std::conditional_t<Index == 3, sf::Event::FocusGained,
        std::conditional_t<Index == 4, sf::Event::TextEntered,
        std::conditional_t<Index == 5, sf::Event::KeyPressed,
        std::conditional_t<Index == 6, sf::Event::KeyReleased,
        std::conditional_t<Index == 7, sf::Event::MouseWheelScrolled,
        std::conditional_t<Index == 8, sf::Event::MouseButtonPressed,
        std::conditional_t<Index == 9, sf::Event::MouseButtonReleased,
        std::conditional_t<Index == 10, sf::Event::MouseMoved,
        std::conditional_t<Index == 11, sf::Event::MouseMovedRaw,
        std::conditional_t<Index == 12, sf::Event::MouseEntered,
        std::conditional_t<Index == 13, sf::Event::MouseLeft,
        std::conditional_t<Index == 14, sf::Event::JoystickButtonPressed,
        std::conditional_t<Index == 15, sf::Event::JoystickButtonReleased,
        std::conditional_t<Index == 16, sf::Event::JoystickMoved,
        std::conditional_t<Index == 17, sf::Event::JoystickConnected,
        std::conditional_t<Index == 18, sf::Event::JoystickDisconnected,
        std::conditional_t<Index == 19, sf::Event::TouchBegan,
        std::conditional_t<Index == 20, sf::Event::TouchMoved,
        std::conditional_t<Index == 21, sf::Event::TouchEnded,
        std::conditional_t<Index == 22, sf::Event::SensorChanged,
        void
        >>>>>>>>>>>>>>>>>>>>>>>>;

    template<typename T>
    constexpr ptrdiff_t eventTypeId =
        std::conditional_t<std::is_same_v<T, EventType<0>>, ptrdiff_t{0},
        std::conditional_t<std::is_same_v<T, EventType<1>>, ptrdiff_t{1},
        std::conditional_t<std::is_same_v<T, EventType<2>>, ptrdiff_t{2},
        std::conditional_t<std::is_same_v<T, EventType<3>>, ptrdiff_t{3},
        std::conditional_t<std::is_same_v<T, EventType<4>>, ptrdiff_t{4},
        std::conditional_t<std::is_same_v<T, EventType<5>>, ptrdiff_t{5},
        std::conditional_t<std::is_same_v<T, EventType<6>>, ptrdiff_t{6},
        std::conditional_t<std::is_same_v<T, EventType<7>>, ptrdiff_t{7},
        std::conditional_t<std::is_same_v<T, EventType<8>>, ptrdiff_t{8},
        std::conditional_t<std::is_same_v<T, EventType<9>>, ptrdiff_t{9},
        std::conditional_t<std::is_same_v<T, EventType<10>>, ptrdiff_t{10},
        std::conditional_t<std::is_same_v<T, EventType<11>>, ptrdiff_t{11},
        std::conditional_t<std::is_same_v<T, EventType<12>>, ptrdiff_t{12},
        std::conditional_t<std::is_same_v<T, EventType<13>>, ptrdiff_t{13},
        std::conditional_t<std::is_same_v<T, EventType<14>>, ptrdiff_t{14},
        std::conditional_t<std::is_same_v<T, EventType<15>>, ptrdiff_t{15},
        std::conditional_t<std::is_same_v<T, EventType<16>>, ptrdiff_t{16},
        std::conditional_t<std::is_same_v<T, EventType<17>>, ptrdiff_t{17},
        std::conditional_t<std::is_same_v<T, EventType<18>>, ptrdiff_t{18},
        std::conditional_t<std::is_same_v<T, EventType<19>>, ptrdiff_t{19},
        std::conditional_t<std::is_same_v<T, EventType<20>>, ptrdiff_t{20},
        std::conditional_t<std::is_same_v<T, EventType<21>>, ptrdiff_t{21},
        std::conditional_t<std::is_same_v<T, EventType<22>>, ptrdiff_t{22},
        ptrdiff_t{-1}>>>>>>>>>>>>>>>>>>>>>>>;

    sf::ConvexShape Line(Vec2f from, Vec2f to, float width = 1);
    sf::CircleShape Circle(float radius, int pointCount = 30);
    sf::Rect<float> Cover(Vec2f a, Vec2f b);
    sf::Rect<float> Cover(sf::Rect<float> a, sf::Rect<float> b);
    sf::RectangleShape Rectangle(sf::Rect<float> rect);
}