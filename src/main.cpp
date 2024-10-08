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
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>

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

// read the fct names
void saveHighScore(int highscore)
{
    std::ofstream outFile("save/highscore.txt");
    if (outFile.is_open())
    {
        outFile << highscore;
        outFile.close();
    }
}

int loadHighScore()
{
    std::ifstream inFile("save/highscore.txt"); //
    int highscore = 0;
    if (inFile.is_open())
    {
        inFile >> highscore;
        inFile.close();
    }

    return highscore;
}

int main()
{
    // game window parameters
    sf::RenderWindow window(sf::VideoMode(800, 800), "Asteroids", sf::Style::Default,
                            sf::ContextSettings(0, 0, 8));

    window.setFramerateLimit(60);

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

    sf::Text HscoreT;
    HscoreT.setFont(font);
    HscoreT.setCharacterSize(30);
    HscoreT.setPosition(30, 130);

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
    sf::RectangleShape pauseBG;

    sf::Clock timer; // delta time(r)
    sf::Clock gameClock;

    sf::Time shootCooldown = sf::seconds(0.2f); // Set your cooldown here
    sf::Time lastShotTime = sf::Time::Zero - shootCooldown;

    int destroyedRocks = 0;
    int maxRocks = 5;
    int highscore = loadHighScore();

    spawnAsteroids(rocks, 3, sf::Vector2f(50, 50), false);

    while (window.isOpen())
    {
        // make the window size unchangeable
        window.setSize(sf::Vector2u(800, 800));

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
            else if (event.type == sf::Event::KeyPressed)
            {
                //pause
                if (!pause && event.key.code == sf::Keyboard::Escape)
                {
                    pause = true;
                    continue;
                }

                // unpause
                else if (pause && event.key.code == sf::Keyboard::Escape)
                {
                    pause = false;
                    continue;
                }

                else if (mainMenu && event.key.code == sf::Keyboard::Enter)
                {
                    mainMenu = false;
                    player.reset();
                    destroyedRocks = 0;
                    bullets.clear();
                    rocks.clear();
                    spawnAsteroids(rocks, 3, sf::Vector2f(50, 50), false);
                    continue;
                }
                else if (gameOver && event.key.code == sf::Keyboard::Enter)
                {
                    gameOver = false;
                    player.reset();
                    destroyedRocks = 0;
                    bullets.clear();
                    rocks.clear();
                    spawnAsteroids(rocks, 3, sf::Vector2f(50, 50), false);
                    continue;
                }
                else if (!gameOver && !pause && !mainMenu)
                {
                    if (event.key.code == sf::Keyboard::Space)
                    {
                        sf::Time currentTime = sf::Time::Zero + gameClock.getElapsedTime();
                        if (currentTime - lastShotTime >= shootCooldown)
                        {
                            sf::Vector2f playerPos = player.getPlayerPos();
                            float playerDir = player.getPlayerDir();

                            bullets.push_back(Bullet(playerPos, playerDir));
                            lastShotTime = currentTime; // Update last shot time
                        }
                    }
                }
            }
        }

        window.clear();

        if (!gameOver && !pause && !mainMenu)
        {
            player.update(deltaTime);
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

                        if (destroyedRocks % 20 == 0)
                        {
                            maxRocks++;
                            std::cout << "max rocks: " << maxRocks << std::endl;
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

                    player.hit();
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

            if (erasedCount > 0 && rocks.size() < maxRocks)
            {
                spawnAsteroids(rocks, 1, sf::Vector2f(50, 50), false);
                //std::cout << "number of rocks: " << rocks.size() << std::endl;
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

            std::string highscoreText = "HighScore:" + std::to_string(highscore);
            HscoreT.setString(highscoreText);

            // display score and health and highscore
            window.draw(healthT);
            window.draw(scoreT);
            window.draw(HscoreT);
        }

        else
        {
            if (gameOver)
            {
                gameStatusText.setString("Game Over!");
                restartText.setString("press Enter to restart");
                gameStatusText.setPosition(100, 300);

                if (destroyedRocks > highscore)
                {
                    highscore = destroyedRocks;
                    saveHighScore(highscore);
                }
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