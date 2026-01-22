#pragma once

// qvm lite documentation link: https://www.boost.org/doc/libs/release/libs/qvm/doc/html/index.html

#include <cstdint>
#include <boost/qvm_lite.hpp>
#include <tuple>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>

namespace spiel
{
    using boost::qvm::mag;
    using boost::qvm::mag_sqr;
    using boost::qvm::normalize;
    using boost::qvm::normalized;
    using boost::qvm::dot;
    using boost::qvm::determinant;
    using boost::qvm::X; 
    using boost::qvm::Y;
    using boost::qvm::Z;
    using boost::qvm::A00;
    using boost::qvm::A01;
    using boost::qvm::A02;
    using boost::qvm::A03;
    using boost::qvm::A10;
    using boost::qvm::A11;
    using boost::qvm::A12;
    using boost::qvm::A13;
    using boost::qvm::A20;
    using boost::qvm::A21;
    using boost::qvm::A22;
    using boost::qvm::A23;
    using boost::qvm::A30;
    using boost::qvm::A31;
    using boost::qvm::A32;
    using boost::qvm::A33;
    using boost::qvm::A;
    using boost::qvm::col; // column of matrix
    using boost::qvm::row; // row of matrix
    using boost::qvm::col_mat; // matrix with col
    using boost::qvm::row_mat; // matrix with row
    using boost::qvm::identity_mat;

    using boost::qvm::vec;
    using boost::qvm::quat;
    using boost::qvm::mat;

    using Vec2f = boost::qvm::vec<float, 2>;
    using Vec2i = boost::qvm::vec<int32_t, 2>;

    using Mat2f = boost::qvm::mat<float, 2, 2>; // matrix * vector = changed vector
    using Vec4f = boost::qvm::vec<float, 4>; // extended vector
    using Mat4f = boost::qvm::mat<float, 4, 4>; // extended matrix * extended vector = changed extended vector

    template<typename T>
    [[nodiscard]] inline boost::qvm::vec<T, 2> operator*(const boost::qvm::vec<T, 2>& complex1, const boost::qvm::vec<T, 2>& complex2)
    {
        return boost::qvm::vec<T, 2>(
            X(complex1) * X(complex2) - Y(complex1) * Y(complex2),
            Y(complex1) * X(complex2) + X(complex1) * Y(complex2)
        );
    }

    template<typename T>
    [[nodiscard]] inline boost::qvm::vec<T, 2>& operator*=(boost::qvm::vec<T, 2>& complexl, const boost::qvm::vec<T, 2>& complexr)
    { return complexl = complexl * complexr; }

    template<typename T>
    [[nodiscard]] inline boost::qvm::vec<T, 2> conjugate(const boost::qvm::vec<T, 2>& complex)
    {
        return boost::qvm::vec<T, 2>(X(complex), -Y(complex));
    }

    template<typename T>
    [[nodiscard]] inline boost::qvm::vec<T, 2> operator/(const boost::qvm::vec<T, 2>& complex1, const boost::qvm::vec<T, 2>& complex2)
    {
        return (complex1 * conjugate(complex2)) / mag_sqr(complex2);
    }

    template<typename T>
    [[nodiscard]] inline boost::qvm::vec<T, 2>& operator/=(boost::qvm::vec<T, 2>& complexl, const boost::qvm::vec<T, 2>& complexr)
    { return complexl = complexl / complexr; }

    namespace vectorCast
    {
        template<typename T>
        [[nodiscard]] inline sf::Vector2<T> toSFML(const boost::qvm::vec<T, 2>& v)
        {
            return sf::Vector2<T>(boost::qvm::get<0>(v), boost::qvm::get<1>(v));
        }

        template<typename T>
        [[nodiscard]] inline boost::qvm::vec<T, 2> fromSFML(const sf::Vector2<T>& v)
        {
            boost::qvm::vec<T, 2> result;
            boost::qvm::set<0>(result, v.x);
            boost::qvm::set<1>(result, v.y);
            return result;
        }

        [[nodiscard]] inline Mat4f fromSFML(const sf::Transform& t)
        {
            Mat4f result;
            A00(result) = t.getMatrix()[0];
            A01(result) = t.getMatrix()[4];
            A02(result) = t.getMatrix()[8];
            A03(result) = t.getMatrix()[12];

            A10(result) = t.getMatrix()[1];
            A11(result) = t.getMatrix()[5];
            A12(result) = t.getMatrix()[9];
            A13(result) = t.getMatrix()[13];

            A20(result) = t.getMatrix()[2];
            A21(result) = t.getMatrix()[6];
            A22(result) = t.getMatrix()[10];
            A23(result) = t.getMatrix()[14];

            A30(result) = t.getMatrix()[3];
            A31(result) = t.getMatrix()[7];
            A32(result) = t.getMatrix()[11];
            A33(result) = t.getMatrix()[15];
            return result;
        }

        [[nodiscard]] inline sf::Transform toSFML(const Mat4f& m)
        {
            return sf::Transform(
                A00(m), A01(m), A03(m),
                A10(m), A11(m), A13(m),
                A20(m), A21(m), A23(m)
            );
        }

        [[nodiscard]] inline std::tuple<Mat2f, Vec2f> splitMatrix(const Mat4f& transform)
        {
            Mat2f linearPart;
            A00(linearPart) = A00(transform);
            A01(linearPart) = A01(transform);
            A10(linearPart) = A10(transform);
            A11(linearPart) = A11(transform);

            Vec2f translationPart;
            X(translationPart) = A03(transform);
            Y(translationPart) = A13(transform);

            return std::make_tuple(std::move(linearPart), std::move(translationPart));
        }

        [[nodiscard]] inline std::tuple<Mat2f, Vec2f> splitMatrix(const sf::Transform& transform)
        {
            Mat2f linearPart;
            A00(linearPart) = transform.getMatrix()[0];
            A01(linearPart) = transform.getMatrix()[4];
            A10(linearPart) = transform.getMatrix()[1];
            A11(linearPart) = transform.getMatrix()[5];

            Vec2f translationPart;
            X(translationPart) = transform.getMatrix()[12];
            Y(translationPart) = transform.getMatrix()[13];

            return std::make_tuple(std::move(linearPart), std::move(translationPart));
        }

        template<typename T>
        [[nodiscard]] inline boost::qvm::vec<T, 2> shrink2d(const boost::qvm::vec<T, 4>& v)
        {
            return {X(v), Y(v)};
        }

        template<typename T>
        [[nodiscard]] inline boost::qvm::vec<T, 4> extend2d00(const boost::qvm::vec<T, 2>& v)
        {
            return {X(v), Y(v), T(0), T(0)};
        }

        template<typename T>
        [[nodiscard]] inline boost::qvm::vec<T, 4> extend2d01(const boost::qvm::vec<T, 2>& v)
        {
            return {X(v), Y(v), T(0), T(1)};
        }

        template<typename T>
        [[nodiscard]] inline boost::qvm::vec<T, 4> extend2d10(const boost::qvm::vec<T, 2>& v = {})
        {
            return {X(v), Y(v), T(1), T(0)};
        }

        template<typename T>
        [[nodiscard]] inline boost::qvm::mat<T, 2, 2> compToMat2(const boost::qvm::vec<T, 2>& complex)
        {
            boost::qvm::mat<T, 2, 2> result;
            A00(result) = X(complex);
            A01(result) = -Y(complex);
            A10(result) = Y(complex);
            A11(result) = X(complex);
            return result;
        }

        template<typename T>
        [[nodiscard]] inline boost::qvm::mat<T, 4, 4> compToMat4(const boost::qvm::vec<T, 2>& complex)
        {
            boost::qvm::mat<T, 4, 4> result = identity_mat<T, 4>();
            A00(result) = X(complex);
            A01(result) = -Y(complex);
            A10(result) = Y(complex);
            A11(result) = X(complex);
            return result;
        }
    }
}