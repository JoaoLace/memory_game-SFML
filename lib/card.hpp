#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class card {
private:
    int id;                      // Identificador único do card (usado para verificar pares)
    bool isFlipped;              // Indica se o card está virado (face visível)
    bool isMatched;              // Indica se o card já foi pareado corretamente
    sf::Sprite sprite;           // Sprite que representa o card na tela
    sf::Texture frontTexture;    // Textura da frente do card (imagem da carta)
    sf::Texture backTexture;     // Textura do verso do card (imagem padrão)

public:
    // Construtores
    card();
    card(int id, const std::string& frontImagePath, const std::string& backImagePath, sf::Vector2f position);

    // Ações principais
    void flip();                 // Vira a carta (para mostrar a frente)
    void unflip();              // Desvira a carta (para mostrar o verso)
    void markMatched();         // Marca a carta como pareada

    // Verificações de clique
    bool contains(sf::Vector2f point) const;   // Verifica se o ponto está dentro do sprite
    bool isClicked(sf::Vector2f point) const;  // Verifica se foi clicado

    // Getters
    bool getIsFlipped() const;
    bool getIsMatched() const;
    int getId() const;

    // Renderização
    void render(sf::RenderWindow& window) const;
    void update(sf::RenderWindow& window) const;

    // Acesso a texturas e sprite
    sf::Texture* getFrontTexture();
    sf::Texture* getBackTexture();
    sf::Sprite& getSprite();
};
