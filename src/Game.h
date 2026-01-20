#pragma once
#include <memory>
#include <array>

#include "MySfml.h"
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
    public:
        GameObject() = default;
        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject(GameObject&&) = delete;
        GameObject& operator=(GameObject&&) = delete;
        ~GameObject() = default;

        template<typename T, typename... Args>
        T& addProperty(Args&&... args)
        {
            auto prop = std::make_unique<T>(*this, std::forward<Args>(args)...);
            T& ref = *prop;
            properties.push_back(std::move(prop));
            return ref;
        }

        Transform2D& getTransform();

        const Transform2D& getTransform() const;
    private:
        std::vector<std::unique_ptr<GameObjectProperty>> properties;
        Transform2D transform;
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
        SubscribtionGuard subscribtion;
    };

    class Transform2D
    {
    public:
        const Comp2f& getPosition() const;
        void setPosition(const Comp2f& positionVal);

        const Comp2f& getRotation() const;
        void setRotation(const Comp2f& rotationVal);

        const Mat2f& getScale() const;
        void setScale(const Mat2f& scaleVal);

        Comp2f getScaleSimple() const;
        void setScaleSimple(const Comp2f& scaleVal);

        Mat2f getMatrix() const;
        sf::Transform getSFTransform() const;
    private:
        Comp2f pos = {0.0f, 0.0f};
        Comp2f rot = {1.0f, 0.0f};
        Mat2f scale = {Comp2f{1.0f, 0.0f}, Comp2f{0.0f, 1.0f}};
    };
}