#pragma once

// qvm lite documentation link: https://www.boost.org/doc/libs/release/libs/qvm/doc/html/index.html

#include <cstdint>
#include <boost/qvm_lite.hpp>
#include <complex>
#include <tuple>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>

namespace spiel
{
    using std::int32_t;
    using std::uint32_t;

    using Vec2f = boost::qvm::vec<float, 2>;
    using Vec2i = boost::qvm::vec<int32_t, 2>;
    using Rot2f = std::complex<float>;

    using Mat2f = boost::qvm::mat<float, 2, 2>; // matrix * vector = changed vector
    using Vec3f = boost::qvm::vec<float, 3>; //
    using Mat3f = boost::qvm::mat<float, 3, 3>; // extended matrix * extended vector = changed extended vector

    inline Vec2f operator*(const Vec2f& vec, const Rot2f& rot)
    {
        return Vec2f(
            vec.a[0] * rot.real() - vec.a[1] * rot.imag(),
            vec.a[0] * rot.imag() + vec.a[1] * rot.real()
        );
    }

     inline Vec2f& operator*=(Vec2f& vec, const Rot2f& rot)
    {
        float x = vec.a[0] * rot.real() - vec.a[1] * rot.imag();
        float y = vec.a[0] * rot.imag() + vec.a[1] * rot.real();
        vec.a[0] = x;
        vec.a[1] = y;
        return vec;
    }

    namespace vectorCast
    {
        inline Vec2f operator*(const Vec2f& vec, const Rot2f& rot)
        {
            return Vec2f(
                vec.a[0] * rot.real() - vec.a[1] * rot.imag(),
                vec.a[0] * rot.imag() + vec.a[1] * rot.real()
            );
        }

        template<typename T>
        inline sf::Vector2<T> toSFML(const boost::qvm::vec<T, 2>& v)
        {
            return sf::Vector2<T>(boost::qvm::get<0>(v), boost::qvm::get<1>(v));
        }

        template<typename T>
        inline boost::qvm::vec<T, 2> fromSFML(const sf::Vector2<T>& v)
        {
            boost::qvm::vec<T, 2> result;
            boost::qvm::set<0>(result, v.x);
            boost::qvm::set<1>(result, v.y);
            return result;
        }

        inline Mat3f fromSFML(const sf::Transform& t)
        {
            Mat3f result;
            result.a[0][0] = t.getMatrix()[0];
            result.a[0][1] = t.getMatrix()[4];
            result.a[0][2] = t.getMatrix()[12];
            result.a[1][0] = t.getMatrix()[1];
            result.a[1][1] = t.getMatrix()[5];
            result.a[1][2] = t.getMatrix()[13];
            result.a[2][0] = t.getMatrix()[3];
            result.a[2][1] = t.getMatrix()[7];
            result.a[2][2] = t.getMatrix()[15];
            return result;
        }

        inline sf::Transform toSFML(const Mat3f& m)
        {
            return sf::Transform(
                m.a[0][0], m.a[0][1], m.a[0][2],
                m.a[1][0], m.a[1][1], m.a[1][2],
                m.a[2][0], m.a[2][1], m.a[2][2]
            );
        }

        inline std::tuple<Mat2f, Vec2f> splitMatrix(const Mat3f& transform)
        {
            Mat2f linearPart;
            linearPart.a[0][0] = transform.a[0][0];
            linearPart.a[0][1] = transform.a[0][1];
            linearPart.a[1][0] = transform.a[1][0];
            linearPart.a[1][1] = transform.a[1][1];

            Vec2f translationPart;
            translationPart.a[0] = transform.a[0][2];
            translationPart.a[1] = transform.a[1][2];

            return std::make_tuple(linearPart, translationPart);
        }

        inline std::tuple<Mat2f, Vec2f> splitMatrix(const sf::Transform& transform)
        {
            return splitMatrix(fromSFML(transform));
        }

        inline Rot2f toRot(const Vec2f& v)
        {
            return Rot2f(v.a[0], v.a[1]);
        }

        inline Vec2f fromRot(const Rot2f& r)
        {
            return Vec2f(r.real(), r.imag());
        }
    }
}