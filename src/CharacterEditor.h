#pragma once
#include "VisualCharacter.h"

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

namespace spiel
{
    class CharacterEditor
    {
    public:
        static void main(sf::RenderWindow& window, tgui::Gui& gui);
    
    private:
    };
}