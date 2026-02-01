#pragma once

#include "optional"

#include "UI.h"

namespace spiel
{
    class Slider : public sf::Drawable
    {
    public:
        Slider(Vec2f from, Vec2f to, bool horizontal = false);

        Slider& setPosition(Vec2f from, Vec2f to, bool horizontal = false);
        Slider& setTrims(bool useTrims);
        Slider& setFloat(float minValue, float maxValue, float initialValue, float lengthDelta);
        Slider& setFloat(float maxValue, float lengthDelta);
        Slider& setInteger(int minValue, int maxValue, int initialValue, int lengthDelta = 2);
        Slider& setInteger(int maxValue, int lengthDelta = 2);

        enum class SliderPart : uint8_t
        {
            Slider,
            BackgroundBar,
            TrimStart,
            TrimEnd
        };

        std::optional<SliderPart> getPartAtPoint(const Vec2f& point) const;
        void drag(const Vec2f& initial, const Vec2f& current);
        void summon(const Vec2f& position);

        inline bool isScrollRequired() const { return maxValue > minValue; }
    private:
        // visual variables
        Vec2f pos;
        float length;
        float sliderLength;
        float thickness;
        float sliderThicknessProportion = 0.8f;
        bool isHorizontal = false;
        bool hasTrims = true;

        SimpleShapeButton<sf::RectangleShape> sliderButton;
        SimpleShapeButton<sf::RectangleShape> backgroundBar;
        SimpleShapeButton<sf::CircleShape> trimStart;
        SimpleShapeButton<sf::CircleShape> trimEnd;

        // backend variables
        float minValue = 0.0f;
        float maxValue;
        float sliderLengthDelta; // not step size, but value difference when slider moves by its length
        float currentValue;
        float delta;
        bool isIntegerOnly = false;
    };
}