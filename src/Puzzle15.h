#pragma once
#include <vector>
#include <complex>
#include <iostream>
#include <SFML/System/Vector2.hpp>

namespace spiel
{
    using Comp2i = std::complex<int>;
    using Comp2f = std::complex<float>;

    template<typename T>
    T& X( std::complex<T>& c ) { return reinterpret_cast<T(&)[2]>(c)[0]; }
    template<typename T>
    T& Y( std::complex<T>& c ) { return reinterpret_cast<T(&)[2]>(c)[1]; }
    template<typename T>
    const T& X( const std::complex<T>& c ) { return reinterpret_cast<const T(&)[2]>(c)[0]; }
    template<typename T>
    const T& Y( const std::complex<T>& c ) { return reinterpret_cast<const T(&)[2]>(c)[1]; }

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

    class Puzzle15
    {
    public:
        explicit inline Puzzle15() : Puzzle15(Comp2i{4, 4}) {}
        Puzzle15(Puzzle15 const&);
        Puzzle15& operator=(Puzzle15 const&);
        Puzzle15(Puzzle15&&);
        Puzzle15& operator=(Puzzle15&&);
        ~Puzzle15() = default;

        Puzzle15(Comp2i size_val);

        static constexpr Comp2i up = {0, -1};
        static constexpr Comp2i down = {0, 1};
        static constexpr Comp2i left = {-1, 0};
        static constexpr Comp2i right = {1, 0};

        bool moveTile(Comp2i direction);

        Comp2i mapIndexToPos(int index) const;
        int mapPosToIndex(Comp2i pos) const;

        Comp2i getSizeCp() const;
        std::vector<int> getBoardCp() const;
        Comp2i getEmptyTilePosCp() const;

        std::vector<int> getBoardWithMaskDist() const;
        std::vector<Comp2i> getBoardWithMaskOffets() const;

        int AtCp(Comp2i pos) const;
        int DistMaskAt(Comp2i pos) const;
        Comp2i OffetrsMaskAt(Comp2i pos) const;

        void shuffle();

        bool isSolved() const;

        bool isSolvable() const;

    private:

        const Comp2i& getSize() const;
        const std::vector<int>& getBoard() const;
        const Comp2i& getEmptyTilePos() const;

        int operator[](Comp2i pos) const;
        int& operator[](Comp2i pos);

        bool isValidPos(Comp2i pos) const;

    private:
        Comp2i size;
        std::vector<int> board;
        Comp2i emptyTilePos;
        const int emptyTileVal;
    public:
        mutable std::ostream* debugOs = nullptr;
    };
} // namespace spiel