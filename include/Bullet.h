#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Bullet
{
public:
    Bullet(bool shoot)
    {
        pew.setRadius(3);
        pew.setPosition(300, 300);
        //
    }

    void drawTo(sf::RenderWindow& window)
    {
        window.draw(pew);
        //
    }

private:
    sf::CircleShape pew;
};