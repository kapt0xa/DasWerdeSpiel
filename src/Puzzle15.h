#pragma once
#include <vector>
#include <complex>
#include <iostream>

#include "MySfml.h"

namespace spiel
{
    class Puzzle15
    {
    public:
        explicit inline Puzzle15() : Puzzle15(Vec2i{4, 4}) {}
        Puzzle15(Puzzle15 const&);
        Puzzle15& operator=(Puzzle15 const&);
        Puzzle15(Puzzle15&&);
        Puzzle15& operator=(Puzzle15&&);
        ~Puzzle15() = default;

        Puzzle15(Vec2i size_val);

        static constexpr Vec2i up = {0, -1};
        static constexpr Vec2i down = {0, 1};
        static constexpr Vec2i left = {-1, 0};
        static constexpr Vec2i right = {1, 0};

        bool moveTile(Vec2i direction);

        Vec2i mapIndexToPos(int index) const;
        int mapPosToIndex(Vec2i pos) const;

        Vec2i getSizeCp() const;
        std::vector<int> getBoardCp() const;
        Vec2i getEmptyTilePosCp() const;

        std::vector<int> getBoardWithMaskDist() const;
        std::vector<Vec2i> getBoardWithMaskOffets() const;

        int AtCp(Vec2i pos) const;
        int DistMaskAt(Vec2i pos) const;
        Vec2i OffetsMaskAt(Vec2i pos) const;

        void shuffle();

        bool isSolved() const;

        bool isSolvable() const;

    private:

        const Vec2i& getSize() const;
        const std::vector<int>& getBoard() const;
        const Vec2i& getEmptyTilePos() const;

        int operator[](Vec2i pos) const;
        int& operator[](Vec2i pos);

        bool isValidPos(Vec2i pos) const;

    private:
        Vec2i size;
        std::vector<int> board;
        Vec2i emptyTilePos;
        const int emptyTileVal;
    public:
        mutable std::ostream* debugOs = nullptr;
    };
} // namespace spiel