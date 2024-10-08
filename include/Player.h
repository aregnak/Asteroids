#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <ctime>
#include <iostream>
#include <math.h>

class Player
{
public:
    Player(float x, float y)
        : _health(10.f)
        , _acceleration(160.f)
        , _rotation(150.f)
        , _velocity(sf::Vector2f(0.0f, 0.0f))
        , _maxspeed(200.f)
        , _isHit(false)
        , up(false)
        , down(false)
        , left(false)
        , right(false)
        , damageCD(sf::seconds(0.1f))
        , currentTime(sf::Time::Zero)
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
        sprite.setColor(sf::Color::White);

        rect.setOrigin(rect.getLocalBounds().width / 2, rect.getLocalBounds().height / 2);
        rect.setPosition(sprite.getPosition().x,
                         sprite.getPosition().y - std::sin(sprite.getRotation()));
    }

    void update(sf::Time& deltaTime)
    {
        sf::Vector2f movement;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            sf::Vector2f direction = sf::Vector2f(std::cos(sprite.getRotation() * M_PI / 180.0f),
                                                  std::sin(sprite.getRotation() * M_PI / 180.0f));

            _velocity -= direction * _acceleration * deltaTime.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            sf::Vector2f direction = sf::Vector2f(std::cos(sprite.getRotation() * M_PI / 180.0f),
                                                  std::sin(sprite.getRotation() * M_PI / 180.0f));

            _velocity += direction * _acceleration * deltaTime.asSeconds();
        }

        // rotation
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            sprite.rotate(-_rotation * deltaTime.asSeconds());
            rect.rotate(-_rotation * deltaTime.asSeconds());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            sprite.rotate(_rotation * deltaTime.asSeconds());
            rect.rotate(_rotation * deltaTime.asSeconds());
        }

        // never surpass max speed
        float _speed = std::sqrt(_velocity.x * _velocity.x + _velocity.y * _velocity.y);
        if (_speed > _maxspeed)
        {
            _velocity *= _maxspeed / _speed;
        }

        //std::cout << "old vel: " << _velocity.x << "    " << _velocity.y << std::endl;

        rect.move(_velocity * deltaTime.asSeconds()); // move hitbox
        sprite.move(_velocity * deltaTime.asSeconds()); // move sprite

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

        //std::cout << clock.getElapsedTime().asSeconds() << std::endl;
        if (_isHit)
        {
            if (clock.getElapsedTime().asSeconds() > currentTime.asSeconds() + damageCD.asSeconds())
            {
                sprite.setColor(sf::Color::White);
                //std::cout << "hit" << std::endl;
            }
            //std::cout << currentTime.asSeconds() << "   " << damageCD.asSeconds() << "    "
            //          << clock.getElapsedTime().asSeconds() << std::endl;
        }
    }

    void hit()
    {
        _isHit = true;
        sprite.setColor(sf::Color::Red);
        currentTime = clock.getElapsedTime();
        //sprite.setColor(sf::Color::White);
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

    void reset()
    {
        _health = 10.f;
        _velocity = sf::Vector2f(0, 0);
        sprite.setPosition(sf::Vector2f(400, 400));
        rect.setPosition(sf::Vector2f(400, 400));
        sprite.setRotation(0.f);
        rect.setRotation(0.f);
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

    bool _isHit;
    sf::Time damageCD;
    sf::Time currentTime;
    sf::Clock clock;

    bool up;
    bool down;
    bool left;
    bool right;
};