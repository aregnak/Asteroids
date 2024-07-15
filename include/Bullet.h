#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>

class Bullet
{
public:
    Bullet(sf::Vector2f position, float angle)
        : velocity(0.5f, 0.5f)
    {
        pew.setRadius(3);
        pew.setPosition(position);

        sf::Vector2f direction =
            sf::Vector2f(std::cos(angle * M_PI / 180.0f), std::sin(angle * M_PI / 180.0f));

        velocity -= direction;
        std::cout << "bullet angle: " << angle << " bullet direction X:" << direction.x
                  << "  Y:" << direction.y << std::endl;
        //
    }

    void update()
    {
        pew.move(velocity / 10.f);
        //
    }

    void drawTo(sf::RenderWindow& window) const
    {
        window.draw(pew);
        //
    }

private:
    sf::CircleShape pew;
    sf::Vector2f velocity;
};