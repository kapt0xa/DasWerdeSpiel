#include "VisualCharacter.h"

namespace spiel
{

    struct RectData : public ShapeData
    {
        Vec2f size;

        std::unique_ptr<sf::Drawable> buildDrawable() const override;
        json::Node toJSON() const override;

        class JSONparser : public ShapeData::JSONparser
        {
        public:
            std::unique_ptr<ShapeData> operator()(const json::Node& node) override;
        };
    };

    struct CircleData : public ShapeData
    {
        Vec2f radius;

        std::unique_ptr<sf::Drawable> buildDrawable() const override;
        json::Node toJSON() const override;

        class JSONparser : public ShapeData::JSONparser
        {
        public:
            std::unique_ptr<ShapeData> operator()(const json::Node& node) override;
        };
    };

    struct EllipseData : public ShapeData
    {
        Vec2f radius;

        std::unique_ptr<sf::Drawable> buildDrawable() const override;
        json::Node toJSON() const override;

        class JSONparser : public ShapeData::JSONparser
        {
        public:
            std::unique_ptr<ShapeData> operator()(const json::Node& node) override;
        };
    };

    struct PolygonData : public ShapeData
    {
        std::vector<Vec2f> points;

        std::unique_ptr<sf::Drawable> buildDrawable() const override;
        json::Node toJSON() const override;

        class JSONparser : public ShapeData::JSONparser
        {
        public:
            std::unique_ptr<ShapeData> operator()(const json::Node& node) override;
        };
    };

    ShapeTransform ShapeTransform::add(const ShapeTransform& parent) const
    {
        return {
            pos + parent.pos,
            rot + parent.rot,
            scale * parent.scale,
            sf::Color(
                uint8_t((color.r * parent.color.r) / 255),
                uint8_t((color.g * parent.color.g) / 255),
                uint8_t((color.b * parent.color.b) / 255),
                uint8_t((color.a * parent.color.a) / 255)
            )
        };
    }

    const std::unordered_map<json::string, std::unique_ptr<ShapeData::JSONparser>> ShapeData::jsonParsers = {
        { "rect", std::make_unique<RectData::JSONparser>() },
        { "circle", std::make_unique<CircleData::JSONparser>() },
        { "ellipse", std::make_unique<EllipseData::JSONparser>() },
        { "polygon", std::make_unique<PolygonData::JSONparser>() }
    };

    std::unique_ptr<sf::Drawable> RectData::buildDrawable() const
    {
        auto rect = std::make_unique<sf::RectangleShape>(vectorCast::toSFML(size));
        auto& rectShape = static_cast<sf::RectangleShape&>(*rect);
        rectShape.setFillColor(color);
        rectShape.setOrigin(vectorCast::toSFML(size) / 2.f);
        rectShape.setRotation(sf::radians(rot));
        rectShape.setPosition(vectorCast::toSFML(pos));
        rectShape.setScale({scale, scale});
        return rect;
    }

    json::Node RectData::toJSON() const
    {
        return json::Builder()
            .StartDict()
                .Key("type").Value("rect")
                .Key("pos").StartArray()
                    .Value(X(pos)).Value(Y(pos)).EndArray()
                .Key("rot").Value(rot)
                .Key("scale").Value(scale)
                .Key("color").StartArray()
                    .Value(color.r).Value(color.g).Value(color.b).Value(color.a).EndArray()
                .Key("size").StartArray()
                    .Value(X(size)).Value(Y(size)).EndArray()
            .EndDict().Build();
    }

    std::unique_ptr<ShapeData> RectData::JSONparser::operator()(const json::Node& node)
    {
        const json::Dict& dataDict = node.AsDict();
        RectData data;
        auto& posArray = dataDict.at("pos").AsArray();
        data.pos = Vec2f(posArray[0].AsDouble(), posArray[1].AsDouble());
        data.rot = dataDict.at("rot").AsDouble();
        data.scale = dataDict.at("scale").AsDouble();
        auto& colorArray = dataDict.at("color").AsArray();
        data.color = sf::Color(
            colorArray[0].AsInt(),
            colorArray[1].AsInt(),
            colorArray[2].AsInt(),
            colorArray[3].AsInt());
        auto& sizeArray = dataDict.at("size").AsArray();
        data.size = Vec2f(sizeArray[0].AsDouble(), sizeArray[1].AsDouble());
        return std::make_unique<RectData>(data);
    }

    std::unique_ptr<sf::Drawable> CircleData::buildDrawable() const
    {
        auto circle = std::make_unique<sf::CircleShape>(radius);
        auto& circleShape = static_cast<sf::CircleShape&>(*circle);
        circleShape.setFillColor(color);
        circleShape.setOrigin(vectorCast::toSFML(radius));
        circleShape.setRotation(sf::radians(rot));
        circleShape.setPosition(vectorCast::toSFML(pos));
        circleShape.setScale({scale, scale});
        return circle;
    }
}