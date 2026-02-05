#include "UI.h"

namespace spiel
{
    void DefaultButton::resolveVisualState()
    {
        if (visualState.isPressed)
            label.setFillColor(colorPalette.pressed);
        else if (visualState.isHovered)
            label.setFillColor(colorPalette.hovered);
        else
            label.setFillColor(colorPalette.usual);
    }

    bool DefaultButton::isInAreaVirt(const Vec2f& point) const
    {
        auto pos = label.getPosition();
        auto halfSize = size * 0.5f;
        return (X(point) >= pos.x - X(halfSize)) && (X(point) <= pos.x + X(halfSize)) &&
               (Y(point) >= pos.y - Y(halfSize)) && (Y(point) <= pos.y + Y(halfSize));
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

    void DefaultButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        sf::RectangleShape background;
        background.setSize(vectorCast::toSFML(size));
        background.setOrigin(background.getSize() * 0.5f);
        background.setFillColor(backgroundColor);
        background.setPosition(getTransformableRef().getPosition());
        background.setRotation(getTransformableRef().getRotation());
        background.setScale(getTransformableRef().getScale());

        target.draw(background, states);
        target.draw(label, states);
    }
}