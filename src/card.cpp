#include "../lib/card.hpp"

// Construtor padrão
card::card() : id(0), isFlipped(false), isMatched(false) {}

// Construtor com parâmetros: define id, carrega as texturas e posiciona o sprite
card::card(int id, const std::string& frontImagePath, const std::string& backImagePath, sf::Vector2f position)
    : id(id), isFlipped(false), isMatched(false) {

    frontTexture.loadFromFile(frontImagePath);
    backTexture.loadFromFile(backImagePath);

    sprite.setTexture(backTexture); // Começa com a imagem de trás
    sprite.setPosition(position);
}

// Vira a carta (se não estiver combinada)
void card::flip() {
    if (!isMatched) {
        isFlipped = true;
        sprite.setTexture(frontTexture);
    }
}

// Desvira a carta (se não estiver combinada)
void card::unflip() {
    if (!isMatched) {
        isFlipped = false;
        sprite.setTexture(backTexture);
    }
}

// Marca a carta como combinada
void card::markMatched() {
    isMatched = true;
}

// Verifica se o ponto está sobre a carta
bool card::contains(sf::Vector2f point) const {
    return sprite.getGlobalBounds().contains(point);
}

// Verifica se foi clicada (e ainda não foi virada nem combinada)
bool card::isClicked(sf::Vector2f point) const {
    return contains(point) && !isFlipped && !isMatched;
}

// Getters
bool card::getIsFlipped() const {
    return isFlipped;
}

bool card::getIsMatched() const {
    return isMatched;
}

int card::getId() const {
    return id;
}

// Renderiza a carta na janela
void card::render(sf::RenderWindow& window) const {
    window.draw(sprite);
}

// Atualização (ia usar, mas nao usei)
void card::update(sf::RenderWindow& window) const {
    
}

// Retorna ponteiros para as texturas (caso precise acessar fora)
sf::Texture* card::getBackTexture() {
    return &backTexture;
}

sf::Texture* card::getFrontTexture() {
    return &frontTexture;
}

// Acesso ao sprite diretamente 
sf::Sprite& card::getSprite() {
    return sprite;
}
