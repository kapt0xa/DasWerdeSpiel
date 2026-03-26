#include "VisualCharacter.h"

namespace spiel
{
    sf::ConvexShape VisualShape::getDrawable() const
    {
        sf::ConvexShape result;
        result.setFillColor(color);
        result.setPointCount(vertices.size());
        for (size_t i = 0; i < vertices.size(); ++i)
            result.setPoint(i, vectorCast::toSFML(vertices[i]));
        return result;
    }
    json::Dict colorToJson(const sf::Color& color)
    {
        return json::Builder().StartDict()
            .Key("r").Value(color.r)
            .Key("g").Value(color.g)
            .Key("b").Value(color.b)
            .Key("a").Value(color.a)
            .EndDict().Build().AsDict();
    }
    sf::Color colorByJson(const json::Node& node)
    {
        const auto& dict = node.AsDict();
        return sf::Color(
            dict.at("r").AsInt(),
            dict.at("g").AsInt(),
            dict.at("b").AsInt(),
            dict.at("a").AsInt()
        );
    }
    json::Dict vect2dToJson(const Vec2f& vec)
    {
        return json::Builder().StartDict()
            .Key("x").Value(X(vec))
            .Key("y").Value(Y(vec))
            .EndDict().Build().AsDict();
    }
    Vec2f vect2dByJson(const json::Node& node)
    {
        const auto& dict = node.AsDict();
        return Vec2f(static_cast<float>(dict.at("x").AsDouble()), static_cast<float>(dict.at("y").AsDouble()));
    }

    json::Node VisualShape::toJson() const
    {
        json::Array verticesArr;
        for (const auto& vertex : vertices)
        {
            verticesArr.push_back(vect2dToJson(vertex));
        }
        return json::Builder().StartDict()
            .Key("color").Value(colorToJson(color))
            .Key("vertices").Value(verticesArr)
            .EndDict().Build();
    }

    VisualShape& VisualShape::byJson(const json::Node& node)
    {
        const auto& dict = node.AsDict();
        color = colorByJson(dict.at("color"));
        const auto& verticesArr = dict.at("vertices").AsArray();
        resizePointArr(0);
        reserve(verticesArr.size());
        for (const auto& vertexNode : verticesArr)
        {
            pushBack(vect2dByJson(vertexNode));
        }
        return *this;
    }
    VisualShape& VisualShape::setColor(const sf::Color& color)
    {
        this->color = color;
        return *this;
    }
    VisualShape& VisualShape::reserve(size_t size)
    {
        vertices.reserve(size);
        return *this;
    }
    VisualShape& VisualShape::resizePointArr(size_t size)
    {
        vertices.resize(size);
        return *this;
    }
    VisualShape& VisualShape::pushBack(const Vec2f& point)
    {
        vertices.push_back(point);
        return *this;
    }

    VisualShape& VisualShape::scale(const Vec2f& scale)
    {
        for (auto& vertex : vertices)
        {
            X(vertex) *= X(scale);
            Y(vertex) *= Y(scale);
        }
        return *this;
    }
    VisualShape& VisualShape::scale(float scale)
    {
        return this->scale(Vec2f(scale, scale));
    }
    VisualShape& VisualShape::rotate(float radians)
    {
        Vec2f complex(std::cos(radians), std::sin(radians));
        return this->rotate(complex);
    }
    VisualShape& VisualShape::rotate(const Vec2f& complex)
    {
        for (auto& vertex : vertices)
        {
            vertex *= complex;
        }
        return *this;
    }
    VisualShape& VisualShape::translate(const Vec2f& translation)
    {
        for (auto& vertex : vertices)
        {
            vertex += translation;
        }
        return *this;
    }
    VisualShape VisualShape::scaled(const Vec2f& scale) const
    {
        VisualShape result = *this;
        result.scale(scale);
        return result;
    }
    VisualShape VisualShape::scaled(float scale) const
    {
        return this->scaled(Vec2f(scale, scale));
    }
    VisualShape VisualShape::rotated(float radians) const
    {
        VisualShape result = *this;
        result.rotate(radians);
        return result;
    }
    VisualShape VisualShape::rotated(const Vec2f& complex) const
    {
        VisualShape result = *this;
        result.rotate(complex);
        return result;
    }
    VisualShape VisualShape::translated(const Vec2f& translation) const
    {
        VisualShape result = *this;
        result.translate(translation);
        return result;
    }
    const sf::Color& VisualShape::getColor() const
    {
        return color;
    }
    sf::Color& VisualShape::getColor()
    {
        return color;
    }
    const Vec2f& VisualShape::getVertex(size_t index) const
    {
        return vertices.at(index);
    }
    Vec2f& VisualShape::getVertex(size_t index)
    {
        return vertices.at(index);
    }
    size_t VisualShape::getVertexCount() const
    {
        return vertices.size();
    }
    VisualShape& VisualShape::setVertex(size_t index, const Vec2f& vertex)
    {
        vertices.at(index) = vertex;
        return *this;
    }

    json::Node VisualCharacter::toJson() const
    {
        if (std::holds_alternative<std::monostate>(data))
        {
            return nullptr;
        }
        else if (std::holds_alternative<VisualShape>(data))
        {
            return std::get<VisualShape>(data).toJson();
        }
        else
        {
            const auto& children = std::get<std::vector<VisualCharacter>>(data);
            json::Array childrenArr;
            for (const auto& child : children)
            {
                childrenArr.push_back(child.toJson());
            }
            return childrenArr;
        }
    }

    VisualCharacter& VisualCharacter::byJson(const json::Node& node)
    {
        if (node.IsNull())
        {
            data = std::monostate{};
        }
        else if (node.IsDict())
        {
            data = VisualShape().byJson(node);
        }
        else if (node.IsArray())
        {
            const auto& childrenArr = node.AsArray();
            std::vector<VisualCharacter> children;
            children.reserve(childrenArr.size());
            for (const auto& childNode : childrenArr)
            {
                children.emplace_back().byJson(childNode);
            }
            data = std::move(children);
        }
        else
        {
            throw std::runtime_error("Invalid JSON for VisualCharacter");
        }
        return *this;
    }
    VisualCharacter& VisualCharacter::setShape(const VisualShape& shape)
    {
        data = shape;
        return *this;
    }
    VisualCharacter& VisualCharacter::setShape(VisualShape&& shape)
    {
        data = std::move(shape);
        return *this;
    }
    VisualCharacter& VisualCharacter::setChildren(const std::vector<VisualCharacter>& children)
    {
        data = children;
        return *this;
    }

    VisualCharacter& VisualCharacter::setName(const std::string& name)
    {
        this->name = name;
        return *this;
    }
    const std::string& VisualCharacter::getName() const
    {
        return name;
    }

    VisualCharacter::operator bool()
    {
        return !
        (
            isNull() ||
            (
                isGroup() &&
                getChildCount() == 0
            )
        );
    }
    bool VisualCharacter::isNull() const
    {
        return std::holds_alternative<std::monostate>(data);
    }
    bool VisualCharacter::isShape() const
    {
        return std::holds_alternative<VisualShape>(data);
    }
    bool VisualCharacter::isGroup() const
    {
        return std::holds_alternative<std::vector<VisualCharacter>>(data);
    }
    const VisualShape& VisualCharacter::getShape() const
    {
        if (isShape())
        {
            return std::get<VisualShape>(data);
        }
        else if (isGroup() && getChildCount() == 1)
        {
            return std::get<std::vector<VisualCharacter>>(data).front().getShape();
        }
        throw std::runtime_error("VisualCharacter does not hold a shape");
    }
    VisualShape& VisualCharacter::getShape()
    {
        return const_cast<VisualShape&>(
        const_cast<const VisualCharacter&>(*this).getShape());
    }
    const VisualCharacter& VisualCharacter::getChild(size_t index) const
    {
        if(isShape() && index == 0)
        {
            return *this;
        }
        if(!isGroup())
        {
            throw std::runtime_error("VisualCharacter does not hold children");
        }
        return std::get<std::vector<VisualCharacter>>(data).at(index);
    }
    VisualCharacter& VisualCharacter::getChild(size_t index)
    {
        return const_cast<VisualCharacter&>(
        const_cast<const VisualCharacter&>(*this).getChild(index));
    }

    VisualCharacter& VisualCharacter::setChild(size_t index, const VisualCharacter& value)
    {
        if(isShape() && index == 0)
        {
            *this = value;
            return *this;
        }
        if(!isGroup())
        {
            throw std::runtime_error("VisualCharacter does not hold children");
        }
        std::get<std::vector<VisualCharacter>>(data).at(index) = value;
        return *this;
    }
    VisualCharacter& VisualCharacter::setChild(size_t index, VisualCharacter&& value)
    {
        if(isShape() && index == 0)
        {
            *this = std::move(value);
            return *this;
        }
        if(!isGroup())
        {
            throw std::runtime_error("VisualCharacter does not hold children");
        }
        std::get<std::vector<VisualCharacter>>(data).at(index) = std::move(value);
        return *this;
    }
    size_t VisualCharacter::getChildCount() const
    {
        if(isNull())
        {
            return 0;
        }
        if(isShape())
        {
            return 1;
        }
        return std::get<std::vector<VisualCharacter>>(data).size();
    }
    VisualCharacter& VisualCharacter::resizeChildren(size_t size)
    {
        if(!isGroup())
        {
            if(isNull())
            {
                data = std::vector<VisualCharacter>();
            }
            else if(isShape())
            {
                data = std::vector<VisualCharacter>{*this};
            }
            else
            {
                throw std::runtime_error("VisualCharacter does not hold children");
            }
        }
        std::get<std::vector<VisualCharacter>>(data).resize(size);
        return *this;
    }
    VisualCharacter& VisualCharacter::pushBackChild(const VisualCharacter& child)
    {
        if(!isGroup())
        {
            resizeChildren(getChildCount());
        }
        std::get<std::vector<VisualCharacter>>(data).push_back(child);
        return *this;
    }
    VisualCharacter& VisualCharacter::reserveChildren(size_t size)
    {
        if(!isGroup())
        {
            resizeChildren(getChildCount());
        }
        std::get<std::vector<VisualCharacter>>(data).reserve(size);
        return *this;
    }

    VisualCharacter& VisualCharacter::setColor(const sf::Color& color)
    {
        if(isShape())
        {
            getShape().setColor(color);
        }
        else if(isGroup())
        {
            for(size_t i = 0; i < getChildCount(); ++i)
            {
                getChild(i).setColor(color);
            }
        }
        return *this;
    }
    VisualCharacter& VisualCharacter::scale(const Vec2f& scale)
    {
        if(isShape())
        {
            getShape().scale(scale);
        }
        else if(isGroup())
        {
            for(size_t i = 0; i < getChildCount(); ++i)
            {
                getChild(i).scale(scale);
            }
        }
        return *this;
    }
    VisualCharacter& VisualCharacter::scale(float scale)
    {
        if(isShape())
        {
            getShape().scale(scale);
        }
        else if(isGroup())
        {
            for(size_t i = 0; i < getChildCount(); ++i)
            {
                getChild(i).scale(scale);
            }
        }
        return *this;
    }
    VisualCharacter& VisualCharacter::rotate(float radians)
    {
        if(isShape())
        {
            getShape().rotate(radians);
        }
        else if(isGroup())
        {
            for(size_t i = 0; i < getChildCount(); ++i)
            {
                getChild(i).rotate(radians);
            }
        }
        return *this;
    }
    VisualCharacter& VisualCharacter::rotate(const Vec2f& complex)
    {
        if(isShape())
        {
            getShape().rotate(complex);
        }
        else if(isGroup())
        {
            for(size_t i = 0; i < getChildCount(); ++i)
            {
                getChild(i).rotate(complex);
            }
        }
        return *this;
    }
    VisualCharacter& VisualCharacter::translate(const Vec2f& translation)
    {
        if(isShape())
        {
            getShape().translate(translation);
        }
        else if(isGroup())
        {
            for(size_t i = 0; i < getChildCount(); ++i)
            {
                getChild(i).translate(translation);
            }
        }
        return *this;
    }