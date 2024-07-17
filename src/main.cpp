#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include "Player.h"
#include "Bullet.h"

int main()
{
    // game window parameters
    sf::RenderWindow window(sf::VideoMode(800, 800), "Asteroids", sf::Style::Default,
                            sf::ContextSettings(0, 0, 50));

    //player size
    Player player(30, 30);
    std::vector<Bullet> bullets;

    while (window.isOpen())
    {
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

                if (event.key.code == sf::Keyboard::Space)
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
            bullet.update();
            bullet.drawTo(window);
        }

        // erase bullets if bullet.erase() is true
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                     [](const Bullet& bullet) { return bullet.erase(); }),
                      bullets.end());

        window.display();
    }

    return 0;
}