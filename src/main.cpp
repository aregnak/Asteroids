#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Entity.h"


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Asteroids", sf::Style::Default,
                            sf::ContextSettings(0, 0, 100));

    Entity something(30, 30);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == event.KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }

                something.processEvent(event.key.code, true);
            }
            else if (event.type == event.KeyReleased)
            {
                something.processEvent(event.key.code, false);
            }
        }

        window.clear();

        something.update();
        something.drawTo(window);

        window.display();
    }

    return 0;
}