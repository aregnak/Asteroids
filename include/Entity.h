#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <math.h>

class Entity
{
public:
    Entity(float x, float y)
    {
        rect.setSize(sf::Vector2f(x, y));

        if (!texture.loadFromFile("res/player.png"))
        {
            std::cout << "Texture loading failed" << std::endl;
            system("pause");
        }

        sprite.setTexture(texture);
        sprite.setScale(0.7f, 0.7f);
        sprite.setPosition(400, 400);
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

        acceleration = 0.0005f;
        _rotation = 0.02f;

        velocity = sf::Vector2f(0.f, 0.f);
        maxspeed = 1.0f;

        up = false;
        down = false;
        left = false;
        right = false;
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

        if (up)
        {
            sf::Vector2f direction = sf::Vector2f(std::cos(sprite.getRotation() * M_PI / 180.0f),
                                                  std::sin(sprite.getRotation() * M_PI / 180.0f));

            velocity -= direction * acceleration;
        }

        if (down)
        {
            sf::Vector2f direction = sf::Vector2f(std::cos(sprite.getRotation() * M_PI / 180.0f),
                                                  std::sin(sprite.getRotation() * M_PI / 180.0f));

            velocity += direction * acceleration;
        }

        if (left)
        {
            sprite.rotate(-_rotation);
        }

        if (right)
        {
            sprite.rotate(_rotation);
        }

        float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        if (speed > maxspeed)
        {
            velocity *= maxspeed / speed;
        }

        rect.move(velocity / 50.0f);
        sprite.move(velocity / 50.0f);
    }

    void drawTo(sf::RenderWindow& window) { window.draw(sprite); }

private:
    sf::RectangleShape rect;
    sf::Texture texture;
    sf::Sprite sprite;

    float acceleration;
    float _rotation;

    sf::Vector2f velocity;
    float maxspeed;

    bool up;
    bool down;
    bool left;
    bool right;
};