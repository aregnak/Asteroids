#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <iostream>
#include <string>
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

    sf::Font font;
    if (!font.loadFromFile("res/font/Hyperspace Bold.otf"))
    {
        std::cout << "failed to load font gg go next" << std::endl;
        system("pause");
    }

    sf::Text gameStatusText;
    gameStatusText.setFont(font);
    gameStatusText.setString("GAME OVER!");
    gameStatusText.setCharacterSize(100);
    gameStatusText.setPosition(100, 300);

    sf::Text restartText;
    restartText.setFont(font);
    restartText.setString("press Enter to restart");
    restartText.setCharacterSize(40);
    restartText.setPosition(120, 450);

    sf::Text healthT;
    healthT.setFont(font);
    healthT.setCharacterSize(40);
    healthT.setPosition(30, 30);

    sf::Text scoreT;
    scoreT.setFont(font);
    scoreT.setCharacterSize(30);
    scoreT.setPosition(30, 80);

    sf::Text helpT;
    helpT.setFont(font);
    helpT.setString(
        "Directions:\nWASD/Arrows to move, space to shoot\nEvery 10 score get 2 health back");
    helpT.setCharacterSize(20);
    helpT.setPosition(100, 350);

    // won't insult your intelligence
    bool gameOver = false;
    bool pause = false;
    bool mainMenu = true;
    int asteroidCnt = 0;
    sf::RectangleShape pauseBG;

    sf::Clock timer;

    // shooting cooldown
    sf::Time shootCD = sf::microseconds(0);
    sf::Time lastShotTime = sf::Time::Zero - shootCD;

    int destroyedRocks = 0;

    spawnAsteroids(rocks, 3, sf::Vector2f(50, 50), false);

    while (window.isOpen())
    {
        // make the window size unchangeable
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
                //pause
                if (!pause && event.key.code == sf::Keyboard::Escape)
                {
                    pause = true;
                    continue;
                }

                // unpause
                if (pause && event.key.code == sf::Keyboard::Escape)
                {
                    pause = false;
                    continue;
                }

                if (mainMenu && event.key.code == sf::Keyboard::Enter)
                {
                    mainMenu = false;
                    player.reset();
                    destroyedRocks = 0;
                    bullets.clear();
                    rocks.clear();
                    spawnAsteroids(rocks, 3, sf::Vector2f(50, 50), false);
                    continue;
                }
                if (gameOver && event.key.code == sf::Keyboard::Enter)
                {
                    gameOver = false;
                    player.reset();
                    destroyedRocks = 0;
                    bullets.clear();
                    rocks.clear();
                    spawnAsteroids(rocks, 3, sf::Vector2f(50, 50), false);
                    continue;
                }

                if (!gameOver && !pause && !mainMenu)
                {
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
            }
            else if (event.type == event.KeyReleased)
            {
                player.processEvent(event.key.code, false);
            }
        }

        window.clear();

        if (!gameOver && !pause && !mainMenu)
        {
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
                        if (rock.canSplit())
                        {
                            rock.setHit();
                            spawnAsteroids(rocks, 2, sf::Vector2f(30, 30), true, rock.getPos());
                            rock.setSplit();
                        }

                        rock.setHit();
                        bullet.setHit();
                        destroyedRocks++;
                        //std::cout << "killed a rock!! rocks destroyed: " << destroyedRocks << std::endl;
                        //std::cout << "NUMBER:: " << destroyedRocks % 10
                        //          << " health: " << player.getHealth() << std::endl;

                        if (destroyedRocks % 10 == 0)
                        {
                            if (player.getHealth() <= 8)
                            {
                                player.setHealth(2.f);
                            }
                            else if (player.getHealth() == 9)
                            {
                                player.setHealth(1.f);
                            }
                        }
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

                    player.takeDmg();
                    //std::cout << "player hit!" << std::endl;
                    //std::cout << "HEALTH: " << player.getHealth() << std::endl;

                    if (player.getHealth() <= 0)
                    {
                        gameOver = true;
                    }
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

            // gets int of health and displays to screen
            int healthFlt = player.getHealth();
            std::string healthStr = "HP:" + std::to_string(healthFlt);
            healthT.setString(healthStr);

            // score int to string
            std::string score = "Score:" + std::to_string(destroyedRocks);
            scoreT.setString(score);

            // display score and health
            window.draw(healthT);
            window.draw(scoreT);
        }
        else
        {
            if (gameOver)
            {
                gameStatusText.setString("Game Over!");
                restartText.setString("press Enter to restart");
                gameStatusText.setPosition(100, 300);
            }
            else if (pause)
            {
                gameStatusText.setString("Paused");
                restartText.setString("");
                gameStatusText.setPosition(100, 200);
                window.draw(helpT);
            }
            else if (mainMenu)
            {
                gameStatusText.setString("Asteroids");
                gameStatusText.setPosition(120, 300);
                restartText.setString("press enter to start");
            }

            window.draw(gameStatusText);
            window.draw(restartText);
        }

        window.display();
    }

    return 0;
}