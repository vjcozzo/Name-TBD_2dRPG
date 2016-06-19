//
// Created by Ryan on 6/9/2016.
//
/* Edited by Vince  (12 June '16) */

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

static float get_magnitude (sf::Vector2<float> vect);

int main (void) {
    int ind = 0;
    float init_x, init_y;
    std::cout << "TESTING TESTING" << std::endl;

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::White);

    sf::Clock clock;
    clock.restart();
/*
    printf ("Please enter the shape's initial x velocity, as a float:");
    scanf ("%f", &init_x);

    printf ("Please enter the shape's initial y velocity, as a float:");
    scanf ("%f", &init_y);

*/
    init_x = 0.0040;
    init_y = 0.0080;
    sf::Vector2<float> player_vel (init_x, init_y);

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
	    {
                window.close();
	    }
        }
/*
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
	    sf::Vector2f newDirec (0, 50);

	    player_vel += newDirec;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
	    sf::Vector2f newDirec (0, -50);

	    player_vel += newDirec;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
	    sf::Vector2f newDirec (-50, 0);

	    player_vel += newDirec;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
	    sf::Vector2f newDirec (50, 0);

	    player_vel += newDirec;
        }
*/

        window.clear();
        if (shape.getPosition().x > 182) {
	    player_vel.x = (-1)*player_vel.x;
/*            shape.move(-200, 0);	*/
        }
        if (shape.getPosition().y > 182) {
	    player_vel.y = (-1)*player_vel.y;
/*            shape.move(0, -200);	*/
        }
        if (shape.getPosition().x < 0) {
	    player_vel.x = (-1)*player_vel.x;
/*            shape.move(200, 0);	*/
        }
        if (shape.getPosition().y < 0) {
	    player_vel.y = (-1)*player_vel.y;
/*            shape.move(0, 200);	*/
        }

	/* Normalize the vector, set a constant magnitude */
	float mag = get_magnitude (player_vel);
	player_vel.x /= mag;
	player_vel.y /= mag;
	player_vel *= 0.00001f/* dt*/;

	shape.move (player_vel.x, player_vel.y);
	if (ind % 10000 == 0) {
	    printf ("Current velocity: <%f, %f> or %f\n", player_vel.x, player_vel.y, mag);
	}
        window.draw(shape);
        window.display();
	ind ++;
    }

    return 0;
}

static float get_magnitude (sf::Vector2<float> vect) {
    float result;

    result = (vect.x * vect.x) + (vect.y * vect.y);
    return result;
}
