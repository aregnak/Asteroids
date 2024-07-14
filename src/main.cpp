#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
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
    Bullet bullet(1);

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
            }

            else if (event.type == event.KeyReleased)
            {
                player.processEvent(event.key.code, false);
            }
        }

        window.clear();

        player.update();
        player.drawTo(window);
        bullet.drawTo(window);

        window.display();
    }

    return 0;
}