#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <complex>

namespace spiel
{
    using Comp2i = std::complex<int>;
    using Comp2f = std::complex<float>;
    using Mat2f = std::array<Comp2f, 2>;

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
    Mat2f RotCompToMat(Comp2f rotation);
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
    };

    int getEventType(sf::Event& event);
    std::string getEventTypeName(sf::Event& event);

    sf::ConvexShape Line(Comp2f from, Comp2f to, float width = 1);
    sf::CircleShape Circle(float radius, int pointCount = 30);
    sf::Rect<float> Cover(sf::Rect<float> a, sf::Rect<float> b);
    sf::RectangleShape Rectangle(sf::Rect<float> rect);
}