#include "../lib/card.hpp"

card::card() : id(0), isFlipped(false), isMatched(false) {}

card::card(int id, const std::string& frontImagePath, const std::string& backImagePath, sf::Vector2f position)
    : id(id), isFlipped(false), isMatched(false) {

    frontTexture.loadFromFile(frontImagePath);
    backTexture.loadFromFile(backImagePath);

    sprite.setTexture(backTexture);
    sprite.setPosition(position);
}

void card::flip() {
    if (!isMatched) {
        isFlipped = true;
        sprite.setTexture(frontTexture);
    }
}

void card::unflip() {
    if (!isMatched) {
        isFlipped = false;
        sprite.setTexture(backTexture);
    }
}

void card::markMatched() {
    isMatched = true;
}

bool card::contains(sf::Vector2f point) const {
    return sprite.getGlobalBounds().contains(point);
}

bool card::isClicked(sf::Vector2f point) const {
    return contains(point) && !isFlipped && !isMatched;
}

bool card::getIsFlipped() const {
    return isFlipped;
}

bool card::getIsMatched() const {
    return isMatched;
}

int card::getId() const {
    return id;
}

void card::render(sf::RenderWindow& window) const {
    window.draw(sprite);
}

void card::update(sf::RenderWindow& window) const{

}

sf::Texture* card::getBackTexture() {
    return &backTexture;
}

sf::Texture* card::getFrontTexture() {
    return &frontTexture;
}


sf::Sprite& card::getSprite() {
    return sprite;
}

