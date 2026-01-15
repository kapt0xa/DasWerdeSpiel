#pragma once
#include <vector>
#include <complex>
#include <iostream>

namespace spiel
{
    using Comp2i = std::complex<int>;
    using Comp2f = std::complex<float>;

    template<typename T1>
    class Cast
    {
    public:
        template<typename T2>
        static std::complex<T1> Complex(const std::complex<T2>& val)
        {
            return std::complex<T1>{static_cast<T1>(val.real()), static_cast<T1>(val.imag())};
        }
    };

    Comp2i x = {1, 0};
    Comp2f y = Cast<float>::Complex(x);

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