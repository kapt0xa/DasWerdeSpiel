#include <iostream>
#include <iomanip>
#include <sstream>
#include <exception>
#include <array>

#include <SFML/Graphics.hpp>
#include "Puzzle15.h"
#include "GameLoop.h"

using namespace spiel;

int gameCode();
void compilerValidation();
std::tuple<int, std::string> getEventType(sf::Event& event);

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

    void handleEvents()
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
            if(moveProgress < 1.0f)
            {
                abortedMoves.push_back(lastMovedTile);
            }
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

        abortedMoves.clear();
        fieldsNeedRedraw = false;
    }
    void drawMovingTile(float deltaTime)
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
    sf::RectangleShape coverFields(Comp2i pos1, Comp2i pos2)
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
    void redraw(Comp2i pos)
    {
        auto eraser = coverFields(pos, pos);
        game.getWindow().draw(eraser);
        sf::Text text(game.getFont(), std::to_string(puzzle.AtCp(pos) + 1), fontSize);
        text.setFillColor(sf::Color::Green);
        text.setPosition({X(pos) * step, Y(pos) * step});
        game.getWindow().draw(text);
    }
    void repairAborted()
    {
        for(auto pos : abortedMoves)
        {
            redraw(pos);
        }
        abortedMoves.clear();
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
    std::vector<Comp2i> abortedMoves;
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

    std::tuple<int, std::string> getEventType(sf::Event& event)
    {
        std::array<const char*, 23> typeNames = {
            "Closed",
            "Resized",
            "FocusLost",
            "FocusGained",
            "TextEntered",
            "KeyPressed",
            "KeyReleased",
            "MouseWheelScrolled",
            "MouseButtonPressed",
            "MouseButtonReleased",
            "MouseMoved",
            "MouseMovedRaw",
            "MouseEntered",
            "MouseLeft",
            "JoystickButtonPressed",
            "JoystickButtonReleased",
            "JoystickMoved",
            "JoystickConnected",
            "JoystickDisconnected",
            "TouchBegan",
            "TouchMoved",
            "TouchEnded",
            "SensorChanged"
        };

        int idx = -1;
        event.visit([&](const auto& sub) {
            using T = std::decay_t<decltype(sub)>;
            if constexpr (std::is_same_v<T, sf::Event::Closed>)                 idx = 0;
            else if constexpr (std::is_same_v<T, sf::Event::Resized>)           idx = 1;
            else if constexpr (std::is_same_v<T, sf::Event::FocusLost>)         idx = 2;
            else if constexpr (std::is_same_v<T, sf::Event::FocusGained>)       idx = 3;
            else if constexpr (std::is_same_v<T, sf::Event::TextEntered>)       idx = 4;
            else if constexpr (std::is_same_v<T, sf::Event::KeyPressed>)        idx = 5;
            else if constexpr (std::is_same_v<T, sf::Event::KeyReleased>)       idx = 6;
            else if constexpr (std::is_same_v<T, sf::Event::MouseWheelScrolled>)idx = 7;
            else if constexpr (std::is_same_v<T, sf::Event::MouseButtonPressed>)idx = 8;
            else if constexpr (std::is_same_v<T, sf::Event::MouseButtonReleased>)idx = 9;
            else if constexpr (std::is_same_v<T, sf::Event::MouseMoved>)        idx = 10;
            else if constexpr (std::is_same_v<T, sf::Event::MouseMovedRaw>)     idx = 11;
            else if constexpr (std::is_same_v<T, sf::Event::MouseEntered>)      idx = 12;
            else if constexpr (std::is_same_v<T, sf::Event::MouseLeft>)         idx = 13;
            else if constexpr (std::is_same_v<T, sf::Event::JoystickButtonPressed>)  idx = 14;
            else if constexpr (std::is_same_v<T, sf::Event::JoystickButtonReleased>) idx = 15;
            else if constexpr (std::is_same_v<T, sf::Event::JoystickMoved>)     idx = 16;
            else if constexpr (std::is_same_v<T, sf::Event::JoystickConnected>) idx = 17;
            else if constexpr (std::is_same_v<T, sf::Event::JoystickDisconnected>) idx = 18;
            else if constexpr (std::is_same_v<T, sf::Event::TouchBegan>)        idx = 19;
            else if constexpr (std::is_same_v<T, sf::Event::TouchMoved>)        idx = 20;
            else if constexpr (std::is_same_v<T, sf::Event::TouchEnded>)        idx = 21;
            else if constexpr (std::is_same_v<T, sf::Event::SensorChanged>)     idx = 22;
        });

        if(idx != -1)
        {
            return {idx, typeNames[idx]};
        }
        else
        {
            return {-1, "Unknown"};
        }
    }