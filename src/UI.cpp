#include "UI.h"

namespace spiel
{
    void DefaultButton::resolveVisualState()
    {
        if (isPressed)
            label.setFillColor(pressedFillColor);
        else if (isHovered)
            label.setFillColor(hoverFillColor);
        else
            label.setFillColor(stdFillColor);
    }

    DefaultButton::DefaultButton(const std::string& text, Vec2f size, unsigned int characterSize)
        : label(getDefaultFont(), text, characterSize)
        , size(size)
    {
        label.setOutlineThickness(1.0f);
        label.setOutlineColor(sf::Color::Black);
        auto textSize = label.getLocalBounds().size;
        label.setOrigin(textSize * 0.5f);
        resolveVisualState();
    }

    void DefaultButton::setVisualHoveredState(bool hovered)
    {
        isHovered = hovered;
        resolveVisualState();
    }

    void DefaultButton::setVisualPressedState(bool pressed)
    {
        isPressed = pressed;
        resolveVisualState();
    }

    bool DefaultButton::isInArea(const Vec2f& point) const
    {
        Vec2f pos = {label.getPosition().x, label.getPosition().y};
        sf::FloatRect bounds(vectorCast::toSFML(pos) - vectorCast::toSFML(size) * 0.5f, vectorCast::toSFML(size));
        return bounds.contains(vectorCast::toSFML(point));
    }
}