
#include "window.hpp"
#include "src/game/game.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

namespace menu::ui
{
    int windowXSize = 800;
    int windowYSize = 600;

    Button::Button(float width, float height, float posX, float posY, sf::Color outlineColor, sf::Color innerColor,
               sf::String buttonText)
    {
        font.loadFromFile("assets/font.ttf");

        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(innerColor);
        shape.setOutlineColor(outlineColor);
        shape.setOutlineThickness(2.0f);
        shape.setPosition((windowXSize - shape.getSize().x) / posX, (windowYSize - shape.getSize().y) / posY);
        
        inner.setFont(font);
        inner.setString(buttonText);
        inner.setCharacterSize(20);
        center_horizontally(shape, inner);
        center_vertically(shape, inner);
        inner.setFillColor(sf::Color::White);

    }
;

    Text::Text(sf::String text, int charSize, float posX, float posY)
    {
        if (font.loadFromFile("assets/font.ttf"))
        {
            inner.setFont(font);
            inner.setString(text);
            inner.setCharacterSize(charSize);
            inner.setPosition(posX, posY);
            inner.setFillColor(sf::Color::White);
        }
    }


    Choice run()
    {
        int selectedButton = 1;

        sf::RenderWindow window(sf::VideoMode(windowXSize, windowYSize), "buhh", sf::Style::Default);
        sf::Image icon;
        

        if (icon.loadFromFile("assets/icon.png"))
        {
            window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        }
        else
        {
            std::cout << "Failed to load icon image." << std::endl;
            return Choice::Quit;
        }

        window.setFramerateLimit(144);
        window.setVerticalSyncEnabled(true);

        Button playButton(  200, 50, 
                            2, 2, 
                            sf::Color::White, sf::Color(150, 150, 150),
                            "pl�"); 

        Button quitButton(  200, 50, 
                            2, 1.5, 
                            sf::Color::Red, sf::Color(150, 0, 0),
                            "quit");

        Text menuText("Main Menu", 25, 330, 75);

        while (window.isOpen())
        {
            // gestion d'�v�nements
            sf::Event event;
            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                    case sf::Event::Closed:
                        // Demande de fermeture (croix de fermeture, alt+f4, etc.)
                        window.close();
                        std::cout << "window has been closed" << std::endl;
                        return Choice::Quit;

                    case sf::Event::KeyPressed:
                        if (event.key.code == sf::Keyboard::Escape)
                        {
                            window.close();
                            std::cout << "Escape key pressed, closing window" << std::endl;
                            return Choice::Quit;
                        }

                        if (event.key.code == sf::Keyboard::Enter && selectedButton == 1)
                        {
                            window.close();
                            game::main::run();
                            return Choice::Start;
                        }
                        else if (event.key.code == sf::Keyboard::Enter && selectedButton == 0)
                        {
                            window.close();
                            std::cout << "Quit button selected, closing window" << std::endl;
                            return Choice::Quit;
                        }

                        if (event.key.code == sf::Keyboard::Z || event.key.code == sf::Keyboard::Up)
                        {
                            if (selectedButton < 1)
                            {
                                selectedButton += 1;
                            }
                        }

                        if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
                        {
                            if (selectedButton > 0)
                            {
                                selectedButton -= 1;
                            }
                        }
                        std::cout << selectedButton << std::endl;

                        if (selectedButton == 1)
                        {
                            playButton.shape.setFillColor(sf::Color(0,150,0));
                            playButton.shape.setOutlineColor(sf::Color::Green);

                            quitButton.shape.setFillColor(sf::Color(150, 0, 0));
                            quitButton.shape.setOutlineColor(sf::Color::Red);
                        }

                        else if (selectedButton == 0)
                        {
                            quitButton.shape.setFillColor(sf::Color(0, 150, 0));
                            quitButton.shape.setOutlineColor(sf::Color::Green);

                            playButton.shape.setFillColor(sf::Color(150, 150, 150));
                            playButton.shape.setOutlineColor(sf::Color::White);
                        }
                }
            }
            
            window.clear(sf::Color::Black);

            window.draw(menuText.inner);

            window.draw(playButton.shape);
            window.draw(playButton.inner);
            window.draw(quitButton.shape);
            window.draw(quitButton.inner);

            window.display(); // Affichage de la nouvelle frame
        }
        return Choice::Quit;
    }
} // namespace menu::ui
