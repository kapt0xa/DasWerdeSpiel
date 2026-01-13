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
int loopTest();

int main()
{
    std::cout << "programm started" << std::endl;
    int x;
    try
    {
        x = loopTest();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        x = -1;
    }
    std::cout << "programm finished with " << x << std::endl;
    return x;
}

GameLoop globalLoop = {};

class A
{
    float t = 0;
    int i = 0;
public:
    void print(float dt)
    {
        std::cout << (t+=dt) << ' ' << i++ << std::endl;
        if(t > 10)
        {
            auto& loop = globalLoop;

            auto stopper = [&loop](){loop.stopTicks();};

            loop.addToQueue(std::move(stopper));
        }
    }
};

int loopTest()
{
    A spammer;

    auto& loop = globalLoop;

    loop.subscribeTickFunction([&spammer](float dt){spammer.print(dt);});

    loop.runTicks();

    return 0;
}

int gameCode()
{

    sf::RenderWindow window(sf::VideoMode({800, 800}), "Puzzle 15");

    // Create a graphical text to display
    const sf::Font font("../resrc/UbuntuSansMono[wght].ttf");
    sf::Text renderedText(font, "initial\ntext", 50);
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