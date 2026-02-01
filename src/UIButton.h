#pragma once

#include "UI.h"

namespace spiel
{
    class Button : public sf::Drawable
    {
    protected:
        struct ColorPalette
        {
            sf::Color usual;
            sf::Color hovered;
            sf::Color pressed;
        };

        struct VisualState
        {
            bool isHovered;
            bool isPressed;
        };

        virtual const VisualState& getVisualStateRef() const = 0;

        virtual const ColorPalette& getColorPaletteRef() const = 0;
        virtual void resolveVisualState() = 0;

        virtual bool isInAreaVirt(const Vec2f& point) const = 0;

    public:
        virtual ~Button() = default;

        bool isInArea(const Vec2f& point) const { return isInAreaVirt(point); }

        inline void setVisualHoveredState(bool hovered) { const_cast<bool&>(getVisualStateRef().isHovered) = hovered; resolveVisualState(); }
        inline void setVisualPressedState(bool pressed) { const_cast<bool&>(getVisualStateRef().isPressed) = pressed; resolveVisualState(); }
        inline bool getVisualHoveredState() const { return getVisualStateRef().isHovered; }
        inline bool getVisualPressedState() const { return getVisualStateRef().isPressed; }

        inline void setStandardColor(const sf::Color& color) { const_cast<sf::Color&>(getColorPaletteRef().usual) = color; }
        inline void setHoverColor(const sf::Color& color) { const_cast<sf::Color&>(getColorPaletteRef().hovered) = color; }
        inline void setPressedColor(const sf::Color& color) { const_cast<sf::Color&>(getColorPaletteRef().pressed) = color; }

        inline sf::Color getStandardColor() const { return getColorPaletteRef().usual; }
        inline sf::Color getHoverColor() const { return getColorPaletteRef().hovered; }
        inline sf::Color getPressedColor() const { return getColorPaletteRef().pressed; }

        virtual Vec2f getSize() const = 0;
        virtual Button& setSize(const Vec2f& size) = 0;
        virtual Vec2f getPos() const = 0;
        virtual Button& setPos(const Vec2f& pos) = 0;
    };

    // final class
    class DefaultButton final : public Button
    {
    public:
        DefaultButton(const std::string& text, Vec2f size, unsigned int characterSize = 30);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        inline sf::Text& getText() { return label; }
        inline const sf::Text& getText() const { return label; }

        inline sf::Color& getBackgroundColor() { return backgroundColor; }
        inline const sf::Color& getBackgroundColor() const { return backgroundColor; }

        inline Vec2f getSize() const override{ return size; }
        inline Button& setSize(const Vec2f& newSize) override { size = newSize; return *this; }
        inline Vec2f getPos() const override { return vectorCast::fromSFML(label.getPosition()); }
        inline Button& setPos(const Vec2f& pos) override { label.setPosition(vectorCast::toSFML(pos)); return *this; }

    protected:
        inline const VisualState& getVisualStateRef() const override { return visualState; }

        inline const ColorPalette& getColorPaletteRef() const override { return colorPalette; }
        void resolveVisualState() override;
        inline bool isInAreaVirt(const Vec2f& point) const override;

    private:
        sf::Text label;
        Vec2f size;
        VisualState visualState = {false, false};
        ColorPalette colorPalette = {sf::Color::Green, sf::Color::Cyan, sf::Color::Blue};
        sf::Color backgroundColor = sf::Color(127, 127, 127, 15);
    };

    template<typename ShapeType>
    class SimpleShapeButton final : public Button, private ShapeType
    {
    public:
        inline SimpleShapeButton(ShapeType&& shape):
            ShapeType(std::move(shape))
        {}

        inline void draw(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(static_cast<const ShapeType&>(*this), states); }

        inline Vec2f getSize() const override { return vectorCast::fromSFML(ShapeType::getGlobalBounds().size); }
        inline Button& setSize(const Vec2f& size) override;
        inline Vec2f getPos() const override { return vectorCast::fromSFML(ShapeType::getPosition()); }
        inline Button& setPos(const Vec2f& pos) override { ShapeType::setPosition(vectorCast::toSFML(pos)); return *this; }

    protected:
        inline const VisualState& getVisualStateRef() const override { return visualState; }
        inline const ColorPalette& getColorPaletteRef() const override { return colorPalette; }
        void resolveVisualState() override;
        inline bool isInAreaVirt(const Vec2f& point) const override;
    private:
        VisualState visualState = {false, false};
        ColorPalette colorPalette = {sf::Color::Green, sf::Color::Cyan, sf::Color::Blue};
    };

    template<typename ShapeType>
    void SimpleShapeButton<ShapeType>::resolveVisualState()
    {
        if (visualState.isPressed)
            ShapeType::setFillColor(colorPalette.pressed);
        else if (visualState.isHovered)
            ShapeType::setFillColor(colorPalette.hovered);
        else
            ShapeType::setFillColor(colorPalette.usual);
    }

    template<typename ShapeType>
    bool SimpleShapeButton<ShapeType>::isInAreaVirt(const Vec2f& point) const
    {
        auto inverseTransform = ShapeType::getInverseTransform();
        auto localPoint = inverseTransform.transformPoint(vectorCast::toSFML(point));
        return ShapeType::getGlobalBounds().contains(localPoint);
    }

    template<typename ShapeType>
    Button& SimpleShapeButton<ShapeType>::setSize(const Vec2f& size)
    {
        auto currentBounds = ShapeType::getLocalBounds();
        ShapeType::setScale({X(size) / currentBounds.size.x, Y(size) / currentBounds.size.y});
        return *this;
    }
}