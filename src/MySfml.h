#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <complex>

namespace spiel
{
    using Comp2i = std::complex<int>;
    using Comp2f = std::complex<float>;
    using Mat2f = std::array<Comp2f, 2>;

    using SimpleEvent = std::function<void(void)>;
    using TickEvent = std::function<void(float deltaTime)>;

    // float ieee754 limits
    constexpr float floatMinNormal = 1.175494351e-38f; // 2^-126
    constexpr float floatMinDenormal = 1.401298e-45f;   // 2^-149
    constexpr float floatRelativeEpsilon = 1.192092896e-07f;  // 2^-23
    constexpr float floatHalfSmall = 1.0842022e-19f; // 2^-63

    template<typename T>
    T& X( std::complex<T>& c ) { return reinterpret_cast<T(&)[2]>(c)[0]; }
    template<typename T>
    T& Y( std::complex<T>& c ) { return reinterpret_cast<T(&)[2]>(c)[1]; }
    template<typename T>
    const T& X( const std::complex<T>& c ) { return reinterpret_cast<const T(&)[2]>(c)[0]; }
    template<typename T>
    const T& Y( const std::complex<T>& c ) { return reinterpret_cast<const T(&)[2]>(c)[1]; }
    Mat2f rotCompToMat(Comp2f rotation);
    Mat2f operator*(const Mat2f& a, const Mat2f& b);
    float det(const Mat2f& m);
    float dot(const Comp2f& a, const Comp2f& b);
    Mat2f inverse(const Mat2f& m);
    Comp2f operator*(const Comp2f& v, const Mat2f& m);
    Comp2f& operator*=(Comp2f& v, const Mat2f& m);

    Comp2f normalized(Comp2f vec, Comp2f defaultVec = {1.0f, 0.0f});
    float magnitude(Comp2f vec);

    template<typename T1 = void>
    class Cast
    {
    public:
        template<typename T2>
        static std::complex<T1> vect(const std::complex<T2>& val)
        {
            return std::complex<T1>{static_cast<T1>(X(val)), static_cast<T1>(Y(val))};
        }
        template<typename T2>
        static sf::Vector2<T1> vect(const sf::Vector2<T2>& val)
        {
            return sf::Vector2<T1>{static_cast<T1>(val.x), static_cast<T1>(val.y)};
        }
        template<typename T2>
        inline static const std::complex<T2>& FromSF(const sf::Vector2<T2>& val)
        {
            return *reinterpret_cast<const std::complex<T2>&>(val);
        }
        template<typename T2>
        inline static std::complex<T2>& FromSF(sf::Vector2<T2>& val)
        {
            return reinterpret_cast<std::complex<T2>&>(val);
        }
        template<typename T2>
        inline static const sf::Vector2<T2>& ToSF(const std::complex<T2>& val)
        {
            return reinterpret_cast<const sf::Vector2<T2>&>(val);
        }
        template<typename T2>
        inline static sf::Vector2<T2>& ToSF(std::complex<T2>& val)
        {
            return reinterpret_cast<sf::Vector2<T2>&>(val);
        }
        inline static sf::Transform ToSFTransform(const Mat2f& m, const Comp2f& offset)
        {
            return sf::Transform(
                X(m[0]), Y(m[0]), X(offset),
                X(m[1]), Y(m[1]), Y(offset),
                0.0f,    0.0f,    1.0f
            );
        }
        inline static std::tuple<Mat2f, Comp2f> FromSFTransform(const sf::Transform& t)
        {
            return {
                Mat2f{
                    Comp2f{t.getMatrix()[0], t.getMatrix()[3]},
                    Comp2f{t.getMatrix()[1], t.getMatrix()[4]}
                },
                Comp2f{t.getMatrix()[2], t.getMatrix()[5]}
            };
        }
    };

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

    sf::ConvexShape Line(Comp2f from, Comp2f to, float width = 1);
    sf::CircleShape Circle(float radius, int pointCount = 30);
    sf::Rect<float> Cover(Comp2f a, Comp2f b);
    sf::Rect<float> Cover(sf::Rect<float> a, sf::Rect<float> b);
    sf::RectangleShape Rectangle(sf::Rect<float> rect);
}