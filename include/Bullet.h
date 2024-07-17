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

        initialPos = pew.getPosition();
        maxDist = 399.0f; // set maximum travel distance before erase of bullet (px)

        // debug stuff
        std::cout << "bullet angle: " << angle << " x vel: " << velocity.x
                  << " y vel: " << velocity.y << std::endl;
    }

    void update()
    {
        // shooting "animaiton"
        pew.move(-velocity / 10.f);
        //
        sf::Vector2f newPos = pew.getPosition();

        // keep bullet in screen
        if (newPos.x < 0)
        {
            newPos.x = 800;
        }
        else if (newPos.x > 800)
        {
            newPos.x = 0;
        }
        if (newPos.y < 0)
        {
            newPos.y = 800;
        }
        else if (newPos.y > 800)
        {
            newPos.y = 0;
        }
        pew.setPosition(newPos);
    }

    void drawTo(sf::RenderWindow& window) const
    {
        // draw pew on screen
        window.draw(pew);
    }

    bool erase() const
    {
        //sf::Vector2f currentPos = pew.getPosition();
        //float distance = std::sqrt(std::pow(currentPos.x - initialPos.x, 2) +
        //                           std::pow(currentPos.y - initialPos.y, 2));

        sf::Vector2f currentPos = pew.getPosition();
        float dx = std::min(std::abs(currentPos.x - initialPos.x),
                            800.0f - std::abs(currentPos.x - initialPos.x));
        float dy = std::min(std::abs(currentPos.y - initialPos.y),
                            800.0f - std::abs(currentPos.y - initialPos.y));
        float distance = std::sqrt(dx * dx + dy * dy);
        return distance > maxDist;
    }

private:
    sf::CircleShape pew;
    sf::Vector2f velocity;
    float maxDist;
    sf::Vector2f initialPos;
};