#include "Game.h"

namespace spiel
{
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
}