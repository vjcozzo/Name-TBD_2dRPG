//
// Created by Ryan on 6/9/2016.
//

#include <iostream>
#include <SFML/Graphics.hpp>

int main (void) {
    std::cout << "TESTING TESTING" << std::endl;


    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock clock;
    clock.restart();

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            shape.move(0, -50 * dt);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            shape.move(0, 50 * dt);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            shape.move(-50 * dt, 0);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            shape.move(50 * dt, 0);
        }

        if(shape.getPosition().x > 200) {
            shape.move(-200, 0);
        }
        else if(shape.getPosition().y > 200) {
            shape.move(0, -200);
        }/*
        if(shape.getPosition().x < 0) {
            shape.move(200, 0);
        }
        if(shape.getPosition().x > 0) {
            shape.move(0, 200);
        }*/

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}