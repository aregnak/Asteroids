#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

class Entity
{
public:
    Entity(float x, float y)
    {
        rect.setSize(sf::Vector2f(x, y));
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
            movement.y -= 0.1f;
        if (down)
            movement.y += 0.1f;
        if (left)
            movement.x -= 0.1f;
        if (right)
            movement.x += 0.1f;

        rect.move(movement/5.0f);
    }


    void drawTo(sf::RenderWindow &window)
    {
        window.draw(rect);
    }
private:
    sf::RectangleShape rect;

    bool up;
    bool down;
    bool left;
    bool right;
};