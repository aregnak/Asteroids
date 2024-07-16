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
    {
        // get shooting direction
        float radianAngle = angle * M_PI / 180.0f;
        velocity = sf::Vector2f(std::cos(radianAngle), std::sin(radianAngle));

        pew.setRadius(3);
        pew.setPosition(position);
        pew.setRotation(angle);

        // debug stuff
        std::cout << "bullet angle: " << angle << " x vel: " << velocity.x
                  << " y vel: " << velocity.y << std::endl;
    }

    void update()
    {
        // shooting "animaiton"
        pew.move(-velocity / 10.f);
    }

    void drawTo(sf::RenderWindow& window) const
    {
        // draw pew on screen
        window.draw(pew);
    }

private:
    sf::CircleShape pew;
    sf::Vector2f velocity;
};