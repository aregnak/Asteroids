#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>

class Asteroid
{
public:
    Asteroid(sf::Vector2f rockSize, bool split, sf::Vector2f position = sf::Vector2f())
        : _isHit(false)
        , _isSplit(split)
        , _size(rockSize)
    {
        if (position == sf::Vector2f())
        {
            // Randomly spawn around the border
            int side = rand() % 4;
            switch (side)
            {
                case 0:
                    _position = sf::Vector2f(rand() % 800, 0);
                    break; // top
                case 1:
                    _position = sf::Vector2f(rand() % 800, 800);
                    break; // bottom
                case 2:
                    _position = sf::Vector2f(0, rand() % 800);
                    break; // left
                case 3:
                    _position = sf::Vector2f(800, rand() % 800);
                    break; // right
            }
        }
        else
        {
            _position = position;
        }
        if (!textureLoaded)
        {
            if (!texture.loadFromFile("res/asteroid_test.png"))
            {
                std::cout << "Asteroid texture loading failed" << std::endl;
                system("pause");
            }
            textureLoaded = true;
            std::cout << textureLoaded << std::endl;
        }

        rock.setTexture(&texture);

        rock.setSize(rockSize);
        rock.setPosition(_position);
        rock.setOrigin(rock.getSize().x / 2, rock.getSize().y / 2);

        _velocity = sf::Vector2f((rand() % 200) - 100, (rand() % 200) - 100);

        _rotateA = static_cast<float>(rand() % 200 - 100);

        _initialPos = rock.getPosition();
    }

    void update(sf::Time deltaTime)
    {
        rock.move(_velocity * deltaTime.asSeconds());

        rock.rotate(_rotateA * deltaTime.asSeconds());

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
        // std::cout << _isSplit << std::endl;
    }

    void drawTo(sf::RenderWindow& window) const
    {
        window.draw(rock);
        //
    }

    bool erase() const
    {
        /*sf::Vector2f currentPos = rock.getPosition();
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
        // keeping this in case i need it another time
*/

        return _isHit;
    }

    bool collision(const sf::Shape& item) const
    {
        return rock.getGlobalBounds().intersects(item.getGlobalBounds());
        //
    }

    void setHit()
    {
        _isHit = true; //
    }

    bool canSplit() const
    {
        return !_isSplit && _size.x > 20 && _size.y > 20; //
    }

    void setSplit()
    {
        _isSplit = true; //
    }

    sf::Vector2f getPos() const
    {
        return rock.getPosition(); //
    }

private:
    sf::RectangleShape rock;

    sf::Vector2f _position;
    sf::Vector2f _velocity;
    sf::Vector2f _initialPos;
    sf::Vector2f _size;
    bool _isHit;
    bool _isSplit;
    float _rotateA;

    static sf::Texture texture;
    static bool textureLoaded;
};