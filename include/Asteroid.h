#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>
#include <ostream>

class Asteroid
{
public:
    Asteroid(sf::Vector2f rockSize)
        //: _position(position)
        : _maxDist(799.f) // max can only be screen width/height -1
    //, _velocity(30, 30)
    {
        int side = rand() % 4;
        switch (side)
        {
            case 0: // Top
                _position = sf::Vector2f(rand() % (800 - 50 + 1) + 50, 50);
                break;
            case 1: // Bottom
                _position = sf::Vector2f(rand() % (800 - 50 + 1) + 50, 350);
                break;
            case 2: // Left
                _position = sf::Vector2f(rand() % (800 - 50 + 1) + 50, 350);
                break;
            case 3: // Right
                _position = sf::Vector2f(rand() % (800 - 50 + 1) + 50, 0);
                break;
        }
        //_position = sf::Vector2f(350, 200);

        rock.setSize(rockSize);
        rock.setPosition(_position);
        rock.setOrigin(10, 10);

        _velocity = sf::Vector2f(100, 100);
        //_velocity = sf::Vector2f(static_cast<float>(rand() % 200 - 100),
        //                         static_cast<float>(rand() % 200 - 100));
        _initialPos = rock.getPosition();

        _initialPos = rock.getPosition();
        //
        //
    }

    void update(sf::Time deltaTime)
    {
        rock.move(_velocity * deltaTime.asSeconds());
        rock.rotate(10.f * deltaTime.asSeconds());

        sf::Vector2f newPos = rock.getPosition();
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
        rock.setPosition(newPos);
    }

    void drawTo(sf::RenderWindow& window) const
    {
        window.draw(rock);
        //
    }

    bool erase() const
    {
        sf::Vector2f currentPos = rock.getPosition();
        float dx = std::abs(currentPos.x - _initialPos.x);
        float dy = std::abs(currentPos.y - _initialPos.y);

        if (currentPos.x < _initialPos.x)
            dx = 800.0f - dx;
        if (currentPos.y < _initialPos.y)
            dy = 800.0f - dy;

        float distance = std::sqrt(dx * dx + dy * dy);

        // std::cout << "Distance: " << distance << ", Max Distance: " << _maxDist
        //           << ", initial pos: " << _initialPos.x << ", current pos: " << currentPos.x
        //           << "   " << dx << " " << dy << std::endl;

        return distance > _maxDist;
    }

    bool collision(const sf::Shape& item) const
    {
        return rock.getGlobalBounds().intersects(item.getGlobalBounds());
        //
    }

private:
    sf::RectangleShape rock;
    sf::Vector2f _position;
    sf::Vector2f _velocity;
    sf::Vector2f _initialPos;
    float _maxDist;
};