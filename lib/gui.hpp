#pragma once

enum textos {titulo, desc, op1, op2, op3, op4};
enum temas {nulo, memes, aleatorio, escolha, speedrun};

// Includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <algorithm>
#include <random>    

sf::Text createText(const sf::Font& font, float size, sf::Vector2f pos, sf::Color color, std::string text);

bool isMouseOverText(const sf::Text& text, const sf::RenderWindow& window);



class gui {
    public:
        sf::Sprite background;
        sf::Texture backgroundTexture;

        std::vector<sf::Text> texts;
        void init(const sf::Font& font);
        void render(sf::RenderTarget &window);
        void update(sf::RenderWindow &window);
        void updateColor(sf::RenderWindow &window);
        int testClick(sf::RenderWindow &window, sf::Event event);
        int testText(sf::RenderWindow &window);

    
};
