#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
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
        _velocity = sf::Vector2f(std::cos(radianAngle), std::sin(radianAngle));

        pew.setRadius(3);
        pew.setPosition(position);

        _initialPos = pew.getPosition();
        _maxDist = 399.0f; // set maximum travel distance before erase of bullet (px)

        // debug stuff
        //std::cout << "bullet angle: " << angle << " x vel: " << _velocity.x
        //          << " y vel: " << _velocity.y << std::endl;
    }

    void update(sf::Time deltaTime)
    {
        // shooting "animaiton"
        pew.move((-_velocity / 10.f) * _bulletSpeed * deltaTime.asSeconds());

        // keep bullet in screen if it goes out
        sf::Vector2f newPos = pew.getPosition();
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
        sf::Vector2f currentPos = pew.getPosition();
        float dx = std::min(std::abs(currentPos.x - _initialPos.x),
                            800.0f - std::abs(currentPos.x - _initialPos.x));
        float dy = std::min(std::abs(currentPos.y - _initialPos.y),
                            800.0f - std::abs(currentPos.y - _initialPos.y));
        float distance = std::sqrt(dx * dx + dy * dy);

        //std::cout << "Distance: " << distance << ", Max Distance: " << _maxDist << std::endl;
        return distance > _maxDist;
    }

    static bool canShoot(sf::Time cooldown)
    {
        static sf::Clock timer;
        if (timer.getElapsedTime() > cooldown)
        {
            timer.restart();
            return true;
        }
        return false;
    }

    const sf::CircleShape& getShape() const
    {
        return pew; //
    }

private:
    sf::CircleShape pew;
    sf::Vector2f _velocity;
    float _maxDist;
    sf::Vector2f _initialPos;
    sf::Vector2f _newPos;

    float _bulletSpeed = 3500.f;
};