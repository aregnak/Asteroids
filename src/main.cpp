#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <iostream>
#include <vector>

#include "Player.h"
#include "Bullet.h"
#include "Asteroid.h"

sf::Texture Asteroid::texture;
bool Asteroid::textureLoaded = false;

void spawnAsteroids(std::vector<Asteroid>& rocks, int count, sf::Vector2f rockSize, bool split,
                    sf::Vector2f position = sf::Vector2f())
{
    for (int i = 0; i < count; i++)
    {
        rocks.push_back(Asteroid(rockSize, split, position));
    }
}

int main()
{
    // game window parameters
    sf::RenderWindow window(sf::VideoMode(800, 800), "Asteroids", sf::Style::Default,
                            sf::ContextSettings(0, 0, 8));

    srand(static_cast<unsigned>(time(0)));

    //player size
    Player player(30, 30);
    std::vector<Bullet> bullets;
    std::vector<Asteroid> rocks;

    sf::Text text;
    text.setString("TESTING");
    //text.setCharacterSize(200);
    //text.setPosition(400, 400);

    sf::Clock timer;

    // shooting cooldown
    sf::Time shootCD = sf::microseconds(0);
    sf::Time lastShotTime = sf::Time::Zero - shootCD;

    int destroyedRocks = 0;

    spawnAsteroids(rocks, 3, sf::Vector2f(50, 50), false);

    while (window.isOpen())
    {
        window.setSize(sf::Vector2u(800, 800));
        sf::Time deltaTime = timer.restart();
        sf::Time shootCD = sf::milliseconds(200);
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
                if (event.key.code == sf::Keyboard::Space &&
                    Bullet::canShoot(lastShotTime, shootCD))
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

            for (Bullet& bullet : bullets)
            {
                if (rock.collision(bullet.getShape()))
                {
                    rock.setHit();
                    if (rock.canSplit())
                    {
                        // split the big rock into 2 small rocks at big rock position
                        rock.setHit();
                        bullet.setHit();
                        spawnAsteroids(rocks, 2, sf::Vector2f(30, 30), true, rock.getPos());
                        rock.setSplit();
                    }
                    destroyedRocks++;
                    std::cout << "killed a rock!! rocks destroyed: " << destroyedRocks << std::endl;
                }
            }

            if (rock.collision(player.getShape()))
            {
                rock.setHit();

                // if it is big rock, -2 hp
                if (!rock.canSplit())
                {
                    player.setHealth(-1.f);
                }

                //if small rock, -1 hp
                else
                {
                    player.setHealth(-2.f);
                }

                std::cout << "player hit!" << std::endl;
                std::cout << "HEALTH: " << player.getHealth() << std::endl;
            }
        }
        auto newEnd = std::remove_if(rocks.begin(), rocks.end(),
                                     [](const Asteroid& rock) { return rock.erase(); });
        int erasedCount = std::distance(newEnd, rocks.end());
        rocks.erase(newEnd, rocks.end());

        if (erasedCount > 0 && rocks.size() < 9)
        {
            spawnAsteroids(rocks, 1, sf::Vector2f(50, 50), false);
            std::cout << "number of rocks: " << rocks.size() << std::endl;
        }

        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                     [](const Bullet& bullet) { return bullet.hitRock(); }),
                      bullets.end());

        window.draw(text);

        window.display();
    }

    return 0;
}