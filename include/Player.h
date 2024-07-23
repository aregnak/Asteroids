#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <math.h>

class Player
{
public:
    Player(float x, float y)
        : _health(1.f)
        , _acceleration(0.0008f)
        , _rotation(0.05f)
        , _velocity(sf::Vector2f(0.0f, 0.0f))
        , _maxspeed(1.5f)
        , up(false)
        , down(false)
        , left(false)
        , right(false)
    {
        rect.setSize(sf::Vector2f(x, y));

        if (!texture.loadFromFile("res/player_new.png"))
        {
            std::cout << "Texture loading failed" << std::endl;
            system("pause");
        }

        // player sprite rendering parameters
        sprite.setTexture(texture);
        sprite.setScale(0.35, 0.35);
        sprite.setPosition(400, 400);
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

        rect.setOrigin(rect.getLocalBounds().width / 2, rect.getLocalBounds().height / 2);
        rect.setPosition(sprite.getPosition().x,
                         sprite.getPosition().y - std::sin(sprite.getRotation()));
    }

    void processEvent(sf::Keyboard::Key key, bool isPressed)
    {
        if (isPressed == true)
        {
            if (key == sf::Keyboard::W || key == sf::Keyboard::Up)
            {
                up = true;
            }

            if (key == sf::Keyboard::S || key == sf::Keyboard::Down)
            {
                down = true;
            }

            if (key == sf::Keyboard::A || key == sf::Keyboard::Left)
            {
                left = true;
            }

            if (key == sf::Keyboard::D || key == sf::Keyboard::Right)
            {
                right = true;
            }
        }
        else
        {
            up = false;
            down = false;
            left = false;
            right = false;
        }
    }

    void update()
    {
        sf::Vector2f movement;

        // thrust movement
        if (up)
        {
            sf::Vector2f direction = sf::Vector2f(std::cos(sprite.getRotation() * M_PI / 180.0f),
                                                  std::sin(sprite.getRotation() * M_PI / 180.0f));

            _velocity -= direction * _acceleration;
        }
        if (down)
        {
            sf::Vector2f direction = sf::Vector2f(std::cos(sprite.getRotation() * M_PI / 180.0f),
                                                  std::sin(sprite.getRotation() * M_PI / 180.0f));

            _velocity += direction * _acceleration;
        }

        // rotation
        if (left)
        {
            sprite.rotate(-_rotation);
            rect.rotate(-_rotation);
        }
        if (right)
        {
            sprite.rotate(_rotation);
            rect.rotate(_rotation);
        }

        // never surpass max speed
        float _speed = std::sqrt(_velocity.x * _velocity.x + _velocity.y * _velocity.y);
        if (_speed > _maxspeed)
        {
            _velocity *= _maxspeed / _speed;
        }

        rect.move(_velocity / 50.0f); // move hitbox
        sprite.move(_velocity / 50.0f); // move sprite

        // teleport player to opposite side if out of play area
        if (rect.getPosition().x <= -15)
        {
            rect.setPosition(rect.getPosition().x + 825, rect.getPosition().y);
            sprite.setPosition(rect.getPosition().x, rect.getPosition().y);
        }
        else if (rect.getPosition().x >= 826)
        {
            rect.setPosition(rect.getPosition().x - 825, rect.getPosition().y);
            sprite.setPosition(rect.getPosition().x, rect.getPosition().y);
        }
        else if (rect.getPosition().y <= -15)
        {
            rect.setPosition(rect.getPosition().x, rect.getPosition().y + 825);
            sprite.setPosition(rect.getPosition().x, rect.getPosition().y);
        }
        else if (rect.getPosition().y >= 826)
        {
            rect.setPosition(rect.getPosition().x, rect.getPosition().y - 825);
            sprite.setPosition(rect.getPosition().x, rect.getPosition().y);
        }
    }

    void drawTo(sf::RenderWindow& window)
    {
        window.draw(sprite);
        //window.draw(rect);
    }

    sf::Vector2f getPlayerPos()
    {
        sf::Vector2f position(rect.getPosition().x, rect.getPosition().y);
        return (position);
    }

    float getPlayerDir()
    {
        return rect.getRotation();
        //
    }
    const sf::RectangleShape& getShape() const
    {
        return rect; //
    }

    float getHealth()
    {
        return _health; //
    }

    float setHealth(float amount)
    {
        return _health += amount; //
    }

private:
    sf::RectangleShape rect; // hitbox of player (slightly smaller rectangle)
    sf::Texture texture; // player texture
    sf::Sprite sprite; // player sprite

    float _health;
    float _acceleration;
    float _rotation;
    float _maxspeed;
    sf::Vector2f _velocity;

    bool up;
    bool down;
    bool left;
    bool right;
};