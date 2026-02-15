#pragma once

#include <SFML/Graphics.hpp>
#include <menu_export.h>

void center_horizontally(const auto& parent, auto& child)
{
    const auto parentBoundingBox = parent.getLocalBounds();
    const sf::Vector2f parentLocalCenter{parentBoundingBox.left + parentBoundingBox.width * 0.5f,
                                         parentBoundingBox.top + parentBoundingBox.height * 0.5f};

    const sf::Vector2f parentGlobalCenter = parent.getTransform().transformPoint(parentLocalCenter);

    const auto childBoundingBox = child.getLocalBounds();
    const auto childOrigin = child.getOrigin();
    child.setOrigin(childBoundingBox.left + childBoundingBox.width * 0.5f, childOrigin.y);

    const auto childPosition = child.getPosition();
    child.setPosition(parentGlobalCenter.x, childPosition.y);
}

void center_vertically(const auto& parent, auto& child)
{
    const auto parentBoundingBox = parent.getLocalBounds();
    const sf::Vector2f parentLocalCenter{parentBoundingBox.left + parentBoundingBox.width * 0.5f,
                                         parentBoundingBox.top + parentBoundingBox.height * 0.5f};

    const sf::Vector2f parentGlobalCenter = parent.getTransform().transformPoint(parentLocalCenter);

    const auto childBoundingBox = child.getLocalBounds();
    const auto childOrigin = child.getOrigin();
    child.setOrigin(childOrigin.x, childBoundingBox.top + childBoundingBox.height * 0.5f);

    const auto childPosition = child.getPosition();
    child.setPosition(childPosition.x, parentGlobalCenter.y);
}

namespace menu::ui
{

    enum class Choice
    {
        Start,
        Quit
    };

    MENU_EXPORT Choice run();

    struct MENU_EXPORT Button
    {
        sf::RectangleShape shape;
        sf::Text inner;
        sf::Font font;

        Button( float width, float height,
                float posX, float posY, 
                sf::Color outlineColor, sf::Color innerColor,
                sf::String buttonText);
    };

    struct MENU_EXPORT Text
    {
        sf::Font font;
        sf::Text inner;
        Text(sf::String text, int charSize, float posX, float posY);
    };



} // namespace menu::ui