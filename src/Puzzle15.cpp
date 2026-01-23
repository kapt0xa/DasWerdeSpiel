#include "Puzzle15.h"
#include <cassert>
#include <random>
#include <ctime>
#include <algorithm>
#include <cmath>

namespace spiel
{
    Puzzle15::Puzzle15(Puzzle15 const& other)
        : size(other.size)
        , board(other.board)
        , emptyTilePos(other.emptyTilePos)
        , emptyTileVal(other.emptyTileVal)
        , debugOs(nullptr)
        {}
    Puzzle15& Puzzle15::operator=(Puzzle15 const& other)
    {
        if(this != &other)
        {
            this->~Puzzle15();
            new (this) Puzzle15(other);
        }
        return *this;
    }
    Puzzle15::Puzzle15(Puzzle15&& other)
        : size(std::move(other.size))
        , board(std::move(other.board))
        , emptyTilePos(std::move(other.emptyTilePos))
        , emptyTileVal(other.emptyTileVal)
        , debugOs(other.debugOs)
        {};
    Puzzle15& Puzzle15::operator=(Puzzle15&& other)
    {
        if(this != &other)
        {
            this->~Puzzle15();
            new (this) Puzzle15(std::move(other));
        }
        return *this;
    }

    Puzzle15::Puzzle15(Vec2i size_val)
        : size(size_val)
        , board(X(size) * Y(size))
        , emptyTilePos({X(size) - 1, Y(size) - 1})
        , emptyTileVal(mapPosToIndex(emptyTilePos))
        , debugOs(nullptr)
    {
        assert(X(size) >= 2 && Y(size) >= 2 && "Puzzle size must be at least 2x2");
        for (int i = 0; i < static_cast<int>(board.size()); ++i)
        {
            board[i] = i;
        }
    }

    bool Puzzle15::moveTile(Vec2i direction)
    {
        Vec2i targetPos = emptyTilePos - direction;
        if(!isValidPos(targetPos))
            return false;

        std::swap((*this)[emptyTilePos], (*this)[targetPos]);
        emptyTilePos = targetPos;
        return true;
    }

    bool Puzzle15::isValidPos(Vec2i pos) const
    {
        return (X(pos) >= 0 && X(pos) < X(size) &&
                Y(pos) >= 0 && Y(pos) < Y(size));
    }

    Vec2i Puzzle15::mapIndexToPos(int index) const
    {
        return {index % X(size), index / X(size)};
    }

    int Puzzle15::mapPosToIndex(Vec2i pos) const
    {
        return static_cast<int>(Y(pos) * X(size) + X(pos));
    }

    const Vec2i& Puzzle15::getSize() const
    {
        return size;
    }

    const std::vector<int>& Puzzle15::getBoard() const
    {
        return board;
    }

    const Vec2i& Puzzle15::getEmptyTilePos() const
    {
        return emptyTilePos;
    }

    void Puzzle15::shuffle()
    {
        auto rng_raw = std::default_random_engine(static_cast<unsigned int>(std::time(nullptr)));
        int fieldSize = static_cast<int>(X(size) * Y(size));
        std::uniform_int_distribution<int> rng(0, fieldSize - 1);

        bool odd_swaps = true;

        // Perform random swaps and count oddity of swaps
        for(int i = 0; i < fieldSize; ++i)
        {
            int swapIndex = rng(rng_raw);
            if(i == swapIndex)
                continue;
            std::swap(board[i], board[swapIndex]);
            odd_swaps = !odd_swaps;
        }

        // locate empty tile
        int emptyTileIndex = static_cast<int>(std::find(board.begin(), board.end(), emptyTileVal) - board.begin());
        auto newEmptyTilePos = mapIndexToPos(emptyTileIndex);

        // count oddity of swaps required to move empty tile to correct position
        auto delta = newEmptyTilePos - emptyTilePos;
        odd_swaps = (X(delta) + Y(delta)) % 2 == 0 ? odd_swaps : !odd_swaps;

        // update empty tile position
        emptyTilePos = newEmptyTilePos;

        // finally fix parity if needed
        if(!odd_swaps)
        {
            // perform a simple swap to fix parity. avoid swapping the empty tile
            if(X(emptyTilePos) != 0)
            {
                std::swap((*this)[Vec2i{0,0}], (*this)[Vec2i{0,1}]);
            }
            else
            {
                std::swap((*this)[Vec2i{1,0}], (*this)[Vec2i{1,1}]);
            }
        }
    }

    int Puzzle15::operator[](Vec2i pos) const
    {
        return const_cast<Puzzle15*>(this)->operator[](pos);
    }

    int& Puzzle15::operator[](Vec2i pos)
    {
        assert(isValidPos(pos) && "Position out of bounds");
        return  board[mapPosToIndex(pos)];
    }

    Vec2i Puzzle15::getSizeCp() const
    {
        return getSize();
    }

    std::vector<int> Puzzle15::getBoardCp() const
    {
        return getBoard();
    }

    Vec2i Puzzle15::getEmptyTilePosCp() const
    {
        return getEmptyTilePos();
    }

    int Puzzle15::DistMaskAt(Vec2i pos) const
    {
        int val = (*this)[pos];

        Vec2i delta = mapIndexToPos(val) - pos;
        return static_cast<int>(std::abs(X(delta)) + std::abs(Y(delta)));
    }

    Vec2i Puzzle15::OffetsMaskAt(Vec2i pos) const
    {
        int val = (*this)[pos];
        return mapIndexToPos(val) - pos;
    }

    std::vector<int> Puzzle15::getBoardWithMaskDist() const
    {
        std::vector<int> maskedBoard;
        maskedBoard.reserve(board.size());

        for(int y = 0; y < Y(size); ++y)
        {
            for(int x = 0; x < X(size); ++x)
            {
                maskedBoard.push_back(DistMaskAt({x, y}));
            }
        }

        return maskedBoard;
    }

    std::vector<Vec2i> Puzzle15::getBoardWithMaskOffets() const
    {
        std::vector<Vec2i> maskedBoard;
        maskedBoard.reserve(board.size());

        for(int y = 0; y < Y(size); ++y)
        {
            for(int x = 0; x < X(size); ++x)
            {
                maskedBoard.push_back(OffetsMaskAt({x, y}));
            }
        }

        return maskedBoard;
    }

    bool Puzzle15::isSolved() const
    {
        for (int i = 0; i < static_cast<int>(board.size()); ++i)
        {
            if (board[i] != (i % static_cast<int>(board.size())))
            {
                return false;
            }
        }
        return true;
    }

    bool Puzzle15::isSolvable() const
    {
        int fieldSize = static_cast<int>(X(size) * Y(size));

        auto boardCopy = board;

        bool odd_swaps = true;
        for(int i = 0; i < fieldSize; ++i)
        {
            if(boardCopy[i] == i)
                continue;
            std::swap(boardCopy[i], boardCopy[boardCopy[i]]);
            odd_swaps = !odd_swaps;
        }

        auto delta = emptyTilePos - mapIndexToPos(emptyTileVal);
        odd_swaps = (X(delta) + Y(delta)) % 2 == 0 ? odd_swaps : !odd_swaps;

        return odd_swaps;
    }

    int Puzzle15::AtCp(Vec2i pos) const
    {
        return (*this)[pos];
    }
}