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
#include <iostream>

int main() {
    // SFML 3 uses sf::Vector2u/sf::Vector2f for sizes instead of two ints
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML 3 + TGUI 1.x");
    
    // TGUI's Gui object handles the interaction between SFML and the widgets
    tgui::GuiSFML gui{window};

    // Create a simple button using TGUI 1.x syntax
    auto button = tgui::Button::create("Click Me!");
    button->setPosition({200, 150});
    button->setSize({200, 50});
    
    // Simple callback when the button is pressed
    button->onPress([]() {
        std::cout << "Button clicked!" << std::endl;
    });

    gui.add(button);

    // Main loop
    while (window.isOpen()) {
        // SFML 3: pollEvent now returns std::optional<sf::Event>
        while (const std::optional event = window.pollEvent()) {
            // Pass the event to TGUI first
            gui.handleEvent(*event);

            // Check for standard window close event
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color(40, 40, 40));
        
        // Draw the GUI widgets
        gui.draw();
        
        window.display();
    }

    return 0;
}
