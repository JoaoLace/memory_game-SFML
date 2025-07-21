#pragma once

// Enums para indexar textos e definir temas
enum textos { titulo, desc, op1, op2, op3, op4, op5 };
enum temas { nulo, memes, aleatorio, escolha, speedrun, online };

// Includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <algorithm>
#include <random>

// Função auxiliar para criar textos com estilo padronizado
sf::Text createText(const sf::Font& font, float size, sf::Vector2f pos, sf::Color color, std::string text);

// Verifica se o mouse está sobre um texto
bool isMouseOverText(const sf::Text& text, const sf::RenderWindow& window);

class gui {
public:
    // Imagem de fundo da interface
    sf::Sprite background;
    sf::Texture backgroundTexture;

    // Vetor de textos da interface
    std::vector<sf::Text> texts;

    // Inicializa os textos e a interface com uma fonte
    void init(const sf::Font& font);

    // Renderiza todos os elementos na tela
    void render(sf::RenderTarget& window);

    // Atualiza o estado da interface (ex: hover do mouse)
    void update(sf::RenderWindow& window);
    void updateColor(sf::RenderWindow& window); // Muda a cor ao passar o mouse

    // Verifica se algum texto foi clicado e retorna seu índice
    int testClick(sf::RenderWindow& window, sf::Event event);

    // Verifica qual texto está sob o cursor
    int testText(sf::RenderWindow& window);
};
