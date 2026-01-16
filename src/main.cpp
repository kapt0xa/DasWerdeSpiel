#include <iostream>
#include <iomanip>
#include <sstream>
#include <exception>

#include <SFML/Graphics.hpp>
#include "Puzzle15.h"
#include "GameLoop.h"

using namespace spiel;

int gameCode();
void compilerValidation();

int main()
{
    return gameCode();
}

class Game
{
public:
    Game& defaultInit();

    Game& setDefaultWindow();
    Game& setWindow(std::unique_ptr<sf::RenderWindow> windowVal);
    sf::RenderWindow& getWindow();

    Game& loadDefaultFont();
    Game& setFont(std::unique_ptr<const sf::Font> fontVal);
    const sf::Font& getFont() const;

    Game& setLoop();
    GameLoop& getLoop();

private:
    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<const sf::Font> font;
    std::unique_ptr<GameLoop> loop;
};

class PuzzleRenderer
{
public:
    PuzzleRenderer(Game& gameRef)
        : game(gameRef), puzzle({4,4}), lastMovedTile{-1,-1}, moveProgress{1.0f}
    {
        redraw();
    }


    void tick(float deltaTime)
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

    TickEvent getTickFunction()
    {
        return [this](float deltaTime)
        {
            this->tick(deltaTime);
        };
    }

    void erase()
    {
        auto& window = game.getWindow();
        auto eraser = sf::RectangleShape(sf::Vector2f(step * puzzle.getSizeCp().real(), step * puzzle.getSizeCp().imag()));
        eraser.setFillColor(sf::Color::Black);
        window.draw(eraser);
    }

private:
    void handleEvents()
    {
        auto& window = game.getWindow();
        auto event = window.pollEvent();

        while(event)
        {
            event->visit(
                [&](const auto e)
                {
                    using EventType = decltype(e);
                    if constexpr (std::is_same_v<EventType, sf::Event::Closed>)
                    {
                        handleClosed();
                    }
                    else if constexpr (std::is_same_v<EventType, sf::Event::KeyPressed>)
                    {
                        handleKeyPressed(e);
                    }
                }
            );
            event = window.pollEvent();
        }
    }
    void handleClosed()
    {
        auto closer = [&]()
        {
            game.getLoop().stopTicks();
            game.getWindow().close();
        };
        game.getLoop().addToQueue(std::move(closer));
    }
    void handleKeyPressed(const sf::Event::KeyPressed& keyEvent)
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
            lastMovedTile = to;
            moveProgress = 0.0f;
        }
    }

    void redraw()
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

        fieldsNeedRedraw = false;
    }
    void drawMovingTile(float deltaTime)
    {
        if(fieldsNeedRedraw)
        {
            redraw();
        }

        Comp2f delta = Cast<float>::vect(lastMovedTile - puzzle.getEmptyTilePosCp());

        // ersse all area of moving tile
        auto eraser = sf::RectangleShape(Cast<>::ToSF((delta + Comp2f{1,1})* step));
        eraser.setFillColor(sf::Color::Black);
        eraser.setPosition({
            std::min(X(lastMovedTile), X(puzzle.getEmptyTilePosCp())) * step,
            std::min(Y(lastMovedTile), Y(puzzle.getEmptyTilePosCp())) * step});
        game.getWindow().draw(eraser);

        // draw moving tile at new position
        auto text = sf::Text(game.getFont(), std::to_string(puzzle.AtCp(lastMovedTile) + 1), fontSize);
        text.setFillColor(sf::Color::Green);
        Comp2f moveOffset = delta * moveProgress;
        Comp2f drawPos = Cast<float>::vect(puzzle.getEmptyTilePosCp()) + moveOffset;
        text.setPosition({X(drawPos) * step, Y(drawPos) * step});
        game.getWindow().draw(text);
    }

private:
    Game& game;
    Puzzle15 puzzle;
    float step = 50;
    uint fontSize = 20;
    Comp2i lastMovedTile;
    float moveProgress = 0.0f;
    float speed = 10;
    bool fieldsNeedRedraw = true;
};


void compilerValidation()
{
#ifdef __GNUC__
#if __GNUC__ == 7 && __GNUC_MINOR__ >= 1 || __GNUC__ > 7
        std::cout << "Compiled with g++ version >= 7.1" << std::endl;
#else
        std::cout << "Compiled with g++ version < 7.1" << std::endl;
#endif
#elifdef __clang__
    std::cout << "Compiled with clang compiler." << std::endl;
#else
    std::cout << "Not using g++/clang compiler." << std::endl;
#endif

    // Check for C++17
#if __cplusplus >= 201703L
    std::cout << "Compiled with C++17 or later." << std::endl;
#else
    std::cout << "Not compiled with C++17." << std::endl;
#endif
}

int gameCode()
{
    compilerValidation();

    Game game;
    game.defaultInit();

    PuzzleRenderer renderer(game);

    game.getLoop().subscribeTickFunction(renderer.getTickFunction());
    game.getLoop().runTicks();

    std::cout << "Game is closed.\n";

    return 0;
}

    Game& Game::defaultInit()
    {
        setDefaultWindow();
        loadDefaultFont();
        setLoop();
        return *this;
    }

    Game& Game::setDefaultWindow()
    {
        auto new_window = std::make_unique<sf::RenderWindow>(sf::VideoMode({800, 800}), "Puzzle 15");
        setWindow(std::move(new_window));
        return *this;
    }
    Game& Game::setWindow(std::unique_ptr<sf::RenderWindow> windowVal)
    {
        std::swap(window, windowVal);
        return *this;
    }
    sf::RenderWindow& Game::getWindow()
    {
        return *window;
    }

    Game& Game::loadDefaultFont()
    {
        auto new_font = std::make_unique<const sf::Font>("../resrc/UbuntuSansMono[wght].ttf");
        setFont(std::move(new_font));
        return *this;
    }
    Game& Game::setFont(std::unique_ptr<const sf::Font> fontVal)
    {
        std::swap(font, fontVal);
        return *this;
    }
    const sf::Font& Game::getFont() const
    {
        return *font;
    }

    Game& Game::setLoop()
    {
        loop = std::make_unique<GameLoop>();
        return *this;
    }

    GameLoop& Game::getLoop()
    {
        return *loop;
    }