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
        : _isHit(false)
        , _maxDist(399.f)
    {
        // get shooting direction
        float radianAngle = angle * M_PI / 180.0f;
        _velocity = sf::Vector2f(std::cos(radianAngle), std::sin(radianAngle));

        pew.setRadius(3);
        pew.setPosition(position);

        _initialPos = pew.getPosition();
    }

    void update(sf::Time deltaTime)
    {
        // shooting "animaiton"
        pew.move((-_velocity / 5.f) * _bulletSpeed * deltaTime.asSeconds());

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

        //std::cout << "Distance: " << distance << ", Max Distance: " << _maxDist << ", dx: " << dx
        //          << ", dy: " << dy << std::endl;

        return distance > _maxDist;
    }

    bool hitRock() const
    {
        return _isHit; //
    }

    static bool canShoot(sf::Time& lastShotTime, sf::Time cooldown)
    {
        sf::Time currentTime =
            sf::Time::Zero + sf::seconds(static_cast<float>(clock()) / CLOCKS_PER_SEC);
        if (currentTime - lastShotTime > cooldown)
        {
            lastShotTime = currentTime;
            return true;
        }
        return false;
    }

    const sf::CircleShape& getShape() const
    {
        return pew; //
    }

    void setHit()
    {
        _isHit = true; //
    }

private:
    sf::CircleShape pew;
    sf::Vector2f _velocity;
    float _maxDist;
    sf::Vector2f _initialPos;
    sf::Vector2f _newPos;

    bool _isHit;

    float _bulletSpeed = 3500.f;
};