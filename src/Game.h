#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include <array>

#include "GameLoop.h"

namespace spiel
{
    class Game
    {
    private:
        Game() = default;
        Game& defaultInit();
    public:
        static Game buildDefaultGame();

        Game& setDefaultWindow();
        Game& setWindow(std::unique_ptr<sf::RenderWindow> windowVal);
        sf::RenderWindow& getWindow();

        Game& loadDefaultFont();
        Game& setFont(std::unique_ptr<const sf::Font> fontVal);
        const sf::Font& getFont() const;

        Game& setLoop();
        GameLoop& getLoop();

    private:
        std::unique_ptr<sf::RenderWindow> window;
        std::unique_ptr<const sf::Font> font;
        std::unique_ptr<GameLoop> loop;
    };

    class GameObjectProperty;
    class Transform2D;
    class GameObject
    {

    };

    class GameObjectProperty
    {
    public:
        explicit GameObjectProperty(GameObject& ownerRef);
        virtual ~GameObjectProperty() = default;
    
        bool subscribe(GameLoop& loop);
        bool unsubscribe();

    protected:
        virtual TickEvent getTickFunction() = 0;
    private:
        GameObject& owner;
    protected:
        SubscribtionGuard subscribtion;
    };

    class Transform2D
    {
    public:
        Transform2D();
        ~Transform2D() = default;

        const Comp2f& getPosition() const;
        void setPosition(const Comp2f& positionVal);

        const Comp2f& getRotation() const;
        void setRotation(const Comp2f& rotationVal);

        const std::array<Comp2f,2>& getScale() const;
        void setScale(const std::array<Comp2f,2>& scaleVal);

        Comp2f getScaleSimple() const;
        void setScaleSimple(const Comp2f& scaleVal);

        std::array<Comp2f,2> getMatrix() const;
    private:
        Comp2f pos;
        Comp2f rot;
        std::array<Comp2f,2> scale;
    };
}