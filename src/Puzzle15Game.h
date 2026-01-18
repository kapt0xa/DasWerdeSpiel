#pragma once
#include "Game.h"
#include "Puzzle15.h"

namespace spiel
{
    // returns int because it mimics int main()
    int puzzleGameCode();

    class PuzzleRenderer
    {
    public:
        PuzzleRenderer(Game& gameRef);

        TickEvent getTickFunction();

        void erase();

    private:

        void tick(float deltaTime);

        void handleEvents();

        void handleClosed();
        void handleKeyPressed(const sf::Event::KeyPressed& keyEvent);

        void redraw();

        void drawMovingTile(float deltaTime);

        sf::RectangleShape coverFields(Comp2i pos1, Comp2i pos2);

        void redraw(Comp2i pos);

        void repairAborted();

    private:
        Game& game;
        Puzzle15 puzzle;
        float step = 50;
        int fontSize = 20;
        Comp2i lastMovedTile;
        float moveProgress = 0.0f;
        float speed = 10;
        bool fieldsNeedRedraw = true;
        std::vector<Comp2i> abortedMoves;
    };
}