#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

#include "Entity.h"


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Asteroids");

    sf::Texture playerModel;

    if (!playerModel.loadFromFile("res/player.png"))
    {
        std::cout << "Texture loading failed" << std::endl;
        system("pause");
    }

    sf::Sprite player;
    player.setTexture(playerModel);
    player.scale(0.7, 0.7);

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
        //window.draw(player);
        something.update();
        something.drawTo(window);
        window.display();
    }

    return 0;
}