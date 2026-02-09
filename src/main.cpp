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
#include <boost/qvm_lite.hpp>

using namespace spiel;

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "TGUI v1.12.0 Test");

    tgui::Gui gui{window};

    auto button = tgui::Button::create("Click to Exit");
    button->setPosition("50%", "50%");
    button->setOrigin(0.5f, 0.5f);
    button->setSize("200", "50");

    button->onPress([&](){ window.close(); });

    button->getRenderer()->setBackgroundColor(sf::Color::Black);
    button->getRenderer()->setTextColor(sf::Color::Green);
    button->getRenderer()->setTextColorHover(sf::Color::Green);
    button->getRenderer()->setBackgroundColorHover(sf::Color(7, 0, 15));
    button->getRenderer()->setTextColorDown(sf::Color::Green);
    button->getRenderer()->setBackgroundColorDown(sf::Color(31, 0, 63));

    gui.add(button);

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if(gui.handleEvent(event.value()))
                continue;

            if (event->getIf<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        gui.draw();
        window.display();
    }
    

    return 0;
}