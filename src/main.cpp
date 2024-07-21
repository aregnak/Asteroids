#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <iostream>

#include "Player.h"
#include "Bullet.h"
#include "Asteroid.h"

int main()
{
    // game window parameters
    sf::RenderWindow window(sf::VideoMode(800, 800), "Asteroids", sf::Style::Default,
                            sf::ContextSettings(0, 0, 50));

    //player size
    Player player(30, 30);
    std::vector<Bullet> bullets;
    std::vector<Asteroid> rocks;

    sf::Clock timer;
    sf::Time shootCD = sf::milliseconds(200); // shooting cooldown

    for (int rockCnt = 0; rockCnt <= 5; rockCnt++)
    {
        rocks.push_back(Asteroid(sf::Vector2f(50, 50)));
    }

    while (window.isOpen())
    {
        sf::Time deltaTime = timer.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            // close window if window is closed
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // escape to close window
            else if (event.type == event.KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }

                // key pressed for player movement
                player.processEvent(event.key.code, true);

                if (event.key.code == sf::Keyboard::Space && Bullet::canShoot(shootCD))
                {
                    sf::Vector2f playerPos = player.getPlayerPos();
                    float playerDir = player.getPlayerDir();

                    bullets.push_back(Bullet(playerPos, playerDir));
                }
            }

            else if (event.type == event.KeyReleased)
            {
                player.processEvent(event.key.code, false);
            }

            //for (int rocksNum; rocksNum < 10; rocksNum++)
            //{

            //}
        }
        window.clear();

        player.update();
        player.drawTo(window);

        for (Bullet& bullet : bullets)
        {
            bullet.update(deltaTime);
            bullet.drawTo(window);
        }

        // erase bullets if bullet.erase() is true
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                     [](const Bullet& bullet) { return bullet.erase(); }),
                      bullets.end());

        for (Asteroid& rock : rocks)
        {
            rock.update(deltaTime);
            rock.drawTo(window);

            for (const Bullet& bullet : bullets)
            {
                if (rock.collision(bullet.getShape()))
                {
                    std::cout << "rock hit" << std::endl;
                }
            }

            if (rock.collision(player.getShape()))
            {
                std::cout << "player hit!" << std::endl;
            }
        }

        rocks.erase(std::remove_if(rocks.begin(), rocks.end(),
                                   [](const Asteroid& rock) { return rock.erase(); }),
                    rocks.end());

        window.display();
    }

    return 0;
}