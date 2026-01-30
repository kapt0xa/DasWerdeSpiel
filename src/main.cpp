#include <iostream>
#include <iomanip>
#include <sstream>
#include <exception>
#include <array>
#include <cmath>
#include <random>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <variant>

#include <SFML/Graphics.hpp>

#include "MySfml.h"
#include "UI.h"
#include <boost/qvm_lite.hpp>

using namespace spiel;

int main()
{
    sf::View view({0.f, 0.f}, {800.f, 600.f});
    sf::RenderWindow window(sf::VideoMode(static_cast<sf::Vector2u>(view.getSize())), "My SFML Window");
    window.setView(view);

    DefaultButton button("Click Me", {200.f, 80.f});
    button.asTransformable().setPosition({0.f, 0.f});

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->getIf<sf::Event::Closed>())
            { window.close(); }
            else if (auto mouseButtonEvent = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if(mouseButtonEvent->button == sf::Mouse::Button::Left)
                {
                    auto pos = window.mapPixelToCoords(mouseButtonEvent->position);
                    if (button.isInArea(vectorCast::fromSFML(pos)))
                    {
                        std::cout << "Button clicked!" << std::endl;
                        button.setVisualPressedState(true);
                    }
                }
            }
            else if (auto mouseMovedEvent = event->getIf<sf::Event::MouseMoved>())
            {
                auto pos = window.mapPixelToCoords(mouseMovedEvent->position);
                if (button.isInArea(vectorCast::fromSFML(pos)))
                {
                    button.setVisualHoveredState(true);
                }
                else
                {
                    button.setVisualHoveredState(false);
                }
            }
            else if (auto mouseButtonReleasedEvent = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if(mouseButtonReleasedEvent->button == sf::Mouse::Button::Left)
                {
                    button.setVisualPressedState(false);
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(button);
        window.display();
    }
}