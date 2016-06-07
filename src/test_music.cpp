/* This is some code to test out capabilities to play music 	   */
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(400, 400), "SFML works!");

/*  Select the song to be played....	        */
/*  In this case some piano improv I was doing. */
    sf::Music test_music;
    if (0 == test_music.openFromFile ("../res/test_music_1.wav")) {
        /* Failure to open file */
        return -1;
    }
    test_music.play();

/*  Now for some text printing */
    sf::Font tFont;
    if (!tFont.loadFromFile ("../res/fonts/UbuntuMono-B_font.ttf")) {
        std::cout << "Trouble loading the font. Now exiting." << std::endl;
        return -1;
    }
    sf::Text msg;
    msg.setFont (tFont);
    msg.setString ("Piano Music");
    msg.setCharacterSize (24);
    msg.setColor (sf::Color::White);
    msg.setStyle (sf::Text::Bold);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw (msg);
        window.display();
    }

    return 0;
}
