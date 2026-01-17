#include "Puzzle15Game.h"

namespace spiel
{
    PuzzleRenderer::PuzzleRenderer(Game& gameRef)
        : game(gameRef), puzzle({4,4}), lastMovedTile{-1,-1}, moveProgress{1.0f}
    {
        redraw();
    }

    TickEvent PuzzleRenderer::getTickFunction()
    {
        return [this](float deltaTime)
        {
            this->tick(deltaTime);
        };
    }

    void PuzzleRenderer::erase() 
    {
        auto& window = game.getWindow();
        auto eraser = sf::RectangleShape(sf::Vector2f(step * puzzle.getSizeCp().real(), step * puzzle.getSizeCp().imag()));
        eraser.setFillColor(sf::Color::Black);
        window.draw(eraser);
    }

    void PuzzleRenderer::tick(float deltaTime)
    {
        handleEvents();

        if(moveProgress < 1)
        {
            moveProgress += deltaTime * speed;
            if(moveProgress > 1.0f) moveProgress = 1.0f;
            drawMovingTile(deltaTime);
        }

        game.getWindow().display();
    }

    void PuzzleRenderer::handleEvents()
    {
        auto& window = game.getWindow();
        auto event = window.pollEvent();

        while(event)
        {
            event->visit(
                [&](const auto e)
                {
                    using EventType = std::decay_t<decltype(e)>;
                    if constexpr (std::is_same_v<EventType, sf::Event::Closed>)
                    {
                        handleClosed();
                    }
                    else if constexpr (std::is_same_v<EventType, sf::Event::KeyPressed>)
                    {
                        handleKeyPressed(e);
                    }
                    else
                    {
                        // unhandled event
                    }
                }
            );
            event = window.pollEvent();
        }
    }

    void PuzzleRenderer::handleClosed()
    {
        auto closer = [&]()
        {
            game.getLoop().stopTicks();
            game.getWindow().close();
        };
        game.getLoop().addToQueue(std::move(closer));
    }

    void PuzzleRenderer::handleKeyPressed(const sf::Event::KeyPressed& keyEvent)
    {
        auto to = puzzle.getEmptyTilePosCp();
        switch (keyEvent.code)
        {
            case sf::Keyboard::Key::Up:
                puzzle.moveTile(Puzzle15::up);
                break;
            case sf::Keyboard::Key::Down:
                puzzle.moveTile(Puzzle15::down);
                break;
            case sf::Keyboard::Key::Left:
                puzzle.moveTile(Puzzle15::left);
                break;
            case sf::Keyboard::Key::Right:
                puzzle.moveTile(Puzzle15::right);
                break;
            case sf::Keyboard::Key::Enter:
                if(puzzle.isSolved())
                    handleClosed();
            default:
                break;
        }
        auto from = puzzle.getEmptyTilePosCp();
        if (from != to)
        {
            if(moveProgress < 1.0f)
            {
                abortedMoves.push_back(lastMovedTile);
            }
            lastMovedTile = to;
            moveProgress = 0.0f;
        }
    }

    void PuzzleRenderer::redraw()
    {
        auto& window = game.getWindow();
        auto eraser = sf::RectangleShape(sf::Vector2f(step * puzzle.getSizeCp().real(), step * puzzle.getSizeCp().imag()));
        eraser.setFillColor(sf::Color::Black);
        auto text = sf::Text(game.getFont(), "", fontSize);
        text.setFillColor(sf::Color::Green);

        fieldsNeedRedraw = false;

        window.draw(eraser);

        auto field =puzzle.getBoardCp();
        auto size = puzzle.getSizeCp();
        auto empty_pos = puzzle.getEmptyTilePosCp();
        for(Comp2i pos = {0, 0}; X(pos) < X(size); ++X(pos))
        {
            for(Y(pos) = 0; Y(pos) < Y(size); ++Y(pos))
            {
                int val = field[static_cast<size_t>(Y(pos) * size.real() + X(pos))];
                if(pos == empty_pos)
                {
                    continue;
                }
                text.setString(std::to_string(val + 1));
                text.setPosition({X(pos) * step, Y(pos) * step});
                window.draw(text);
            }
        }

        abortedMoves.clear();
        fieldsNeedRedraw = false;
    }

    void PuzzleRenderer::drawMovingTile(float deltaTime)
    {
        if(fieldsNeedRedraw)
        { redraw();}
        if(abortedMoves.empty() == false)
        { repairAborted(); }

        Comp2f delta = Cast<float>::vect(lastMovedTile - puzzle.getEmptyTilePosCp());

        // ersse all area of moving tile
        auto eraser = coverFields(lastMovedTile, puzzle.getEmptyTilePosCp());
        game.getWindow().draw(eraser);

        // draw moving tile at new position
        auto text = sf::Text(game.getFont(), std::to_string(puzzle.AtCp(lastMovedTile) + 1), fontSize);
        text.setFillColor(sf::Color::Green);
        Comp2f moveOffset = delta * moveProgress;
        Comp2f drawPos = Cast<float>::vect(puzzle.getEmptyTilePosCp()) + moveOffset;
        text.setPosition({X(drawPos) * step, Y(drawPos) * step});
        game.getWindow().draw(text);
    }

    sf::RectangleShape PuzzleRenderer::coverFields(Comp2i pos1, Comp2i pos2)
    {
        auto rect = sf::RectangleShape (sf::Vector2f{
            (std::abs(X(pos1) - X(pos2)) + 1) * step,
            (std::abs(Y(pos1) - Y(pos2)) + 1) * step
        });
        rect.setPosition(sf::Vector2f{
            std::min(X(pos1), X(pos2)) * step,
            std::min(Y(pos1), Y(pos2)) * step
        });
        rect.setFillColor(sf::Color::Black);
        return rect;
    }

    void PuzzleRenderer::redraw(Comp2i pos)
    {
        auto eraser = coverFields(pos, pos);
        game.getWindow().draw(eraser);
        sf::Text text(game.getFont(), std::to_string(puzzle.AtCp(pos) + 1), fontSize);
        text.setFillColor(sf::Color::Green);
        text.setPosition({X(pos) * step, Y(pos) * step});
        game.getWindow().draw(text);
    }

    void PuzzleRenderer::repairAborted()
    {
        for(auto pos : abortedMoves)
        {
            redraw(pos);
        }
        abortedMoves.clear();
    }

    int puzzleGameCode()
    {
        auto game = Game::buildDefaultGame();

        PuzzleRenderer renderer(game);

        game.getLoop().subscribeTickFunction(renderer.getTickFunction());
        game.getLoop().runTicks();

        std::cout << "Game is closed.\n";

        return 0;
    }
}