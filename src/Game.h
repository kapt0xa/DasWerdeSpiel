#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "GameLoop.h"

namespace spiel
{
    class Game
    {
    private:
        Game() = default;
        Game& defaultInit();
    public:
        static Game buildDefaultGame();

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
}