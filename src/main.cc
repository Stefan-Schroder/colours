#include <SFML/Graphics.hpp>
#include <iostream>

#include "particle_controller.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "Colours", sf::Style::Resize | sf::Style::Close);
    
    sf::Clock clock;

    ParticleController pc(window.getSize());

    bool is_pressed = false;

    while (window.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            window.close();
        }

        is_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

        while(!sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        {
        }

        // Close the window if it is being sent a close signal
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            } else if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }
        // float delta_time = (clock.getElapsedTime().asMicroseconds()) / 1000000.0;
        // clock.restart();
        float delta_time = 0.016667;

        window.clear();
        pc.PhysicsUpdate(delta_time, window, is_pressed);

        pc.DrawParticles(window);
        window.display();

        while(sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        {
        }
    }

    return 0;
}
