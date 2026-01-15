#include <iostream>
#include <iomanip>
#include <sstream>
#include <exception>

#include <SFML/Graphics.hpp>
#include "Puzzle15.h"
#include "GameLoop.h"

using namespace spiel;

void PrintPuzzle(const Puzzle15& puzzle, std::ostream& os = std::cout);

int gameCode();

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
    PuzzleRenderer(Game& gameRef);

    void tick(float deltaTime)
    {
        auto& window = game.getWindow();
        auto event = window.pollEvent();

        while(event)
        {
            if(event->is<sf::Event::Closed>())
            {
                auto closer = [&]()
                {
                    game.getLoop().stopTicks();
                    window.close();
                };
                game.getLoop().addToQueue(std::move(closer));
            }
            auto key_event = event->getIf<sf::Event::KeyPressed>();
            if (key_event)
            {
                auto to = puzzle.getEmptyTilePosCp();
                switch (key_event->code)
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
                            window.close();
                    default:
                        break;
                }
                auto from = puzzle.getEmptyTilePosCp();
                if (from != to)
                {
                    movingChip = std::make_tuple(from, to, 0.0f);
                }
            }
            event = window.pollEvent();
        }

        auto field =puzzle.getBoardCp();
        auto size = puzzle.getSizeCp();
        auto empty_pos = puzzle.getEmptyTilePosCp();

        auto eraser = sf::RectangleShape(sf::Vector2f(step * size.real(), step * size.imag()));
        for(int x = 0; x < size.real(); ++x)
        {
            for(int y = 0; y < size.imag(); ++y)
            {
                Comp2i pos{x, y};
                if(pos == empty_pos) continue;
                if(movingChip)
                {
                    auto& [from, to, progress] = *movingChip;
                    if(pos == to) continue;
                    continue here, write the moving tile later
                }

                int val = field[static_cast<size_t>(y * size.real() + x)];

                auto tile = sf::Text(game.getFont(), std::to_string(val + 1), fontSize);
                tile.setFillColor(sf::Color::Green);
                tile.setPosition({x * step, y * step});
                window.draw(tile);
            }
        }
    }

private:
    Game& game;
    Puzzle15 puzzle;
    float step = 50;
    uint fontSize = 20;
    std::optional<std::tuple<Comp2i /*from*/, Comp2i /*to*/, float/*0..1 progress*/>> movingChip;
    float speed = 10;
};

int gameCode()
{
    Game game;
    game.defaultInit();

    sf::Text renderedText(game.getFont(), "initial\ntext", 50);
    renderedText.setFillColor(sf::Color::Green);

    window.clear();
    renderedText.setString("press digit 2-9\nto choose puzzle sise");
    window.draw(renderedText);
    window.display();

    Puzzle15 puzzle;
    while (window.isOpen())
    {
        auto event = window.pollEvent();
        if(event)
        {
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }
            auto key_event = event->getIf<sf::Event::TextEntered>();
            if (key_event)
            {
                char32_t code = key_event->unicode;
                if(code >= U'2' && code <= U'9')
                {
                    int size = static_cast<int>(code - U'0');
                    puzzle = Puzzle15({size, size});
                    puzzle.shuffle();
                    break;
                }
            }
        }
    }
    
    std::ostringstream oss;

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
            auto key_event = event->getIf<sf::Event::KeyPressed>();
            if (key_event)
            {
                switch (key_event->code)
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
                            window.close();
                    default:
                        break;
                }
            }

            window.clear();
            std::ostringstream oss;
            PrintPuzzle(puzzle, oss);
            renderedText.setString(oss.str());
            window.draw(renderedText);
            window.display();
        }
    }

    if(puzzle.isSolved())
    {
        std::cout << "Congratulations! You solved the puzzle!\n";
    }
    std::cout << "Game is closed.\n";

    return 0;
}

void PrintPuzzle(const Puzzle15& puzzle, std::ostream& os)
{
    Comp2i size = puzzle.getSizeCp();
    const std::vector<int>& board = puzzle.getBoardCp();
    Comp2i empty = puzzle.getEmptyTilePosCp();

    for(int y = 0; y < size.imag(); ++y)
    {
        for(int x = 0; x < size.real(); ++x)
        {
            int val = board[static_cast<size_t>(y * size.real() + x)];
            if(x == empty.real() && y == empty.imag())
            {
                os << std::setw(3) << " ";
            }
            else
            {
                os << std::setw(3) << val + 1;
            }
        }
        os << "\n";
    }
}

    Game& Game::defaultInit()
    {
        setDefaultWindow();
        loadDefaultFont();
        setLoop();
    }

    Game& Game::setDefaultWindow()
    {
        auto new_window = std::make_unique<sf::RenderWindow>(sf::VideoMode({800, 800}), "Puzzle 15");
        setWindow(std::move(new_window));
    }
    Game& Game::setWindow(std::unique_ptr<sf::RenderWindow> windowVal)
    {
        std::swap(window, windowVal);
    }
    sf::RenderWindow& Game::getWindow()
    {
        return *window;
    }

    Game& Game::loadDefaultFont()
    {
        auto new_font = std::make_unique<const sf::Font>("../resrc/UbuntuSansMono[wght].ttf");
        setFont(std::move(new_font));
    }
    Game& Game::setFont(std::unique_ptr<const sf::Font> fontVal)
    {
        std::swap(font, fontVal);
    }
    const sf::Font& Game::getFont() const
    {
        return *font;
    }

    Game& Game::setLoop()
    {
        loop = std::make_unique<GameLoop>();
    }
    GameLoop& Game::getLoop()
    {
        return *loop;
    }