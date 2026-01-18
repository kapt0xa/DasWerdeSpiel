#include "Game.h"

namespace spiel
{
        Game& Game::defaultInit()
    {
        setDefaultWindow();
        loadDefaultFont();
        setLoop();
        return *this;
    }

    Game& Game::setDefaultWindow()
    {
        auto new_window = std::make_unique<sf::RenderWindow>(sf::VideoMode({800, 800}), "Puzzle 15");
        setWindow(std::move(new_window));
        return *this;
    }
    Game& Game::setWindow(std::unique_ptr<sf::RenderWindow> windowVal)
    {
        std::swap(window, windowVal);
        return *this;
    }
    sf::RenderWindow& Game::getWindow()
    {
        return *window;
    }

    Game& Game::loadDefaultFont()
    {
        auto new_font = std::make_unique<const sf::Font>("../resrc/UbuntuSansMono[wght].ttf");
        setFont(std::move(new_font));
        return *this;
    }
    Game& Game::setFont(std::unique_ptr<const sf::Font> fontVal)
    {
        std::swap(font, fontVal);
        return *this;
    }
    const sf::Font& Game::getFont() const
    {
        return *font;
    }

    Game& Game::setLoop()
    {
        loop = std::make_unique<GameLoop>();
        return *this;
    }

    GameLoop& Game::getLoop()
    {
        return *loop;
    }

    Game Game::buildDefaultGame()
    {
        Game game;
        game.defaultInit();
        return game;
    }

    GameObjectProperty::GameObjectProperty(GameObject& ownerRef)
        : owner(ownerRef)
    {
    }

    bool GameObjectProperty::subscribe(GameLoop& loop)
    {
        if (subscribtion.unsubscribe())
        {
            subscribtion = loop.subscribeTickFunction(getTickFunction());
            return true;
        }
        return false;
    }

    bool GameObjectProperty::unsubscribe()
    {
        return subscribtion.unsubscribe();
    }

    const Comp2f& Transform2D::getPosition() const
    {
        return pos;
    }

    void Transform2D::setPosition(const Comp2f& positionVal)
    {
        pos = positionVal;
    }

    const Comp2f& Transform2D::getRotation() const
    {
        return rot;
    }

    void Transform2D::setRotation(const Comp2f& rotationVal)
    {
        rot = rotationVal;
    }

    const Mat2f& Transform2D::getScale() const
    {
        return scale;
    }

    void Transform2D::setScale(const Mat2f& scaleVal)
    {
        scale = scaleVal;
    }

    Comp2f Transform2D::getScaleSimple() const
    {
        return Comp2f{X(scale[0]), Y(scale[1])};
    }

    void Transform2D::setScaleSimple(const Comp2f& scaleVal)
    {
        scale = {Comp2f{X(scaleVal), 0.0f}, Comp2f{0.0f, Y(scaleVal)}};
    }

    Mat2f Transform2D::getMatrix() const
    {
        return rotCompToMat(rot) * scale;
    }

    sf::Transform Transform2D::getSFTransform() const
    {
        return Cast<>::ToSFTransform(getMatrix(), pos);
    }
}