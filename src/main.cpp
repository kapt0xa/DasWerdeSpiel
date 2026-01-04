#include <iostream>
#include <iomanip>
#include <sstream>

#include <SFML/Graphics.hpp>
#include "puzzle15.h"

using namespace spiel;

void PrintPuzzle(const Puzzle15& puzzle, std::ostream& os = std::cout);

int main()
{
    Puzzle15 puzzle;
    std::cout << 
        "initial state:\n"
        "size: " << puzzle.getSizeCp() << "\n"
        "is solved: " << std::boolalpha << puzzle.isSolved() << "\n"
        "is solvable: " << std::boolalpha << puzzle.isSolvable() << "\n"
        "board:\n\n";
    PrintPuzzle(puzzle);
    std::cout << "\nPress enter to continue...\n";
    std::cin.get();

    bool isMappingCorrect = true;
    for (int i = 0; i < 16; ++i)
    {
        auto pos = puzzle.mapIndexToPos(i);
        if(i != puzzle.mapPosToIndex(pos))
        {
            isMappingCorrect = false;
            std::cout << "Mapping functions are incorrect, irrevessable!\n";
            break;
        }
        if(i != puzzle.AtCp(pos))
        {
            isMappingCorrect = false;
            std::cout << "Indexing operator is incorrect!\n";
            break;
        }
    }
    if(isMappingCorrect)
    {
        std::cout << "Mapping functions and indexing operator are correct.\n";
    }
    std::cout << "\nPress enter to continue...\n";
    std::cin.get();

    puzzle.shuffle();
    std::cout << 
        "after shuffling:\n"
        "size: " << puzzle.getSizeCp() << "\n"
        "is solved: " << std::boolalpha << puzzle.isSolved() << "\n"
        "is solvable: " << std::boolalpha << puzzle.isSolvable() << "\n"
        "empty tile pos: " << puzzle.getEmptyTilePosCp() << "\n"
        "board:\n\n";
    PrintPuzzle(puzzle);
    std::cout << "\nPress enter to continue...";
    std::cin.get();

    sf::RenderWindow window(sf::VideoMode({800, 800}), "Puzzle 15");

    // Create a graphical text to display
    const sf::Font font("../resrc/UbuntuSansMono[wght].ttf");
    sf::Text renderedText(font, "initial\ntext", 50);
    renderedText.setFillColor(sf::Color::Green);

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