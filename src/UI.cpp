#include "UI.h"

namespace spiel
{
    const sf::Font& DefaultButton::defaultFont = GetDefaultFont();

    DefaultButton::DefaultButton(const std::string& text, Vec2f size, unsigned int characterSize)
        : label(defaultFont, text, characterSize)
        , size(size)
    {
        label.setOutlineThickness(1.0f);
        label.setOutlineColor(sf::Color::Black);
        auto textSize = label.getLocalBounds().size;
        label.setOrigin(textSize * 0.5f);
    }

    void DefaultButton::setVisualState(bool hovered, bool pressed)
    {
        if (pressed)
            label.setFillColor(pressedFillColor);
        else if (hovered)
            label.setFillColor(hoverFillColor);
        else
            label.setFillColor(stdFillColor);
    }

    bool DefaultButton::isInArea(const Vec2f& point) const
    {
        Vec2f pos = {label.getPosition().x, label.getPosition().y};
        sf::FloatRect bounds(vectorCast::toSFML(pos) - vectorCast::toSFML(size) * 0.5f, vectorCast::toSFML(size));
        return bounds.contains(vectorCast::toSFML(point));
    }
}