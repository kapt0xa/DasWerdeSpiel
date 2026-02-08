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
    // 1. Create the SFML window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "TGUI v1.12.0 Test");

    // 2. Create the GUI object and attach it to the window
    tgui::Gui gui{window};

    // 3. Create a widget (Button)
    auto button = tgui::Button::create("Click to Exit");
    button->setPosition("50%", "50%");
    button->setOrigin(0.5f, 0.5f);
    button->setSize("200", "50");

    // 4. Connect a callback (Lambda)
    button->onPress([&](){ window.close(); });

    // 5. Add the widget to the GUI
    gui.add(button);

    // 6. Start the automated main loop (handles events and drawing)
    gui.mainLoop();

    return 0;
}