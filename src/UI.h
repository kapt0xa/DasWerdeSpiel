#pragma once

#include "MySfml.h"
#include "Vectors.h"

namespace spiel
{
    class Button : public sf::Drawable
    {
    public:
        virtual operator sf::Transformable&() = 0;
        virtual operator const sf::Transformable&() const = 0;

        virtual bool isInArea(const Vec2f& point) const = 0;
        virtual Vec2f getSize() const = 0;
        virtual void setSize(const Vec2f& size) = 0;

        virtual void setVisualState(bool hovered, bool pressed) = 0;

        virtual void setStandardFillColor(const sf::Color& color) = 0;
        virtual void setHoverFillColor(const sf::Color& color) = 0;
        virtual void setPressedFillColor(const sf::Color& color) = 0;

        virtual sf::Color getStandardFillColor() const = 0;
        virtual sf::Color getHoverFillColor() const = 0;
        virtual sf::Color getPressedFillColor() const = 0;

        inline sf::Transformable& asTransformable() { return static_cast<sf::Transformable&>(*this); };
        inline const sf::Transformable& asTransformable() const { return static_cast<const sf::Transformable&>(*this); };
    };

    // final class
    class DefaultButton final : public Button
    {
    private:
        static const sf::Font& defaultFont;
    public:
        DefaultButton(const std::string& text, Vec2f size, unsigned int characterSize = 30);

        inline void draw(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(label, states); }
        void setVisualState(bool hovered, bool pressed) override;
        operator sf::Transformable&() override { return label; }
        operator const sf::Transformable&() const override { return label; }

        bool isInArea(const Vec2f& point) const override;
        inline Vec2f getSize() const override { return size; }
        inline void setSize(const Vec2f& size) override { this->size = size; }

        inline sf::Text& getLabel() { return label; }
        inline const sf::Text& getLabel() const { return label; }

        void setStandardFillColor(const sf::Color& color) override { stdFillColor = color; }
        void setHoverFillColor(const sf::Color& color) override { hoverFillColor = color; }
        void setPressedFillColor(const sf::Color& color) override { pressedFillColor = color; }

        sf::Color getStandardFillColor() const override { return stdFillColor; }
        sf::Color getHoverFillColor() const override { return hoverFillColor; }
        sf::Color getPressedFillColor() const override { return pressedFillColor; }

    private:
        sf::Text label;
        Vec2f size;
        sf::Color stdFillColor = sf::Color::Green;
        sf::Color hoverFillColor = sf::Color::Cyan;
        sf::Color pressedFillColor = sf::Color::Blue;
    };
}