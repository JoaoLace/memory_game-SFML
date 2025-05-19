#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class card {
private:
    int id;
    bool isFlipped;
    bool isMatched;
    sf::Sprite sprite;
    sf::Texture frontTexture;
    sf::Texture backTexture;

public:
    card();
    card(int id, const std::string& frontImagePath, const std::string& backImagePath, sf::Vector2f position);

    void flip();
    void unflip();
    void markMatched();

    bool contains(sf::Vector2f point) const;
    bool isClicked(sf::Vector2f point) const;

    bool getIsFlipped() const;
    bool getIsMatched() const;
    int getId() const;

    void render(sf::RenderWindow& window) const;
    void update(sf::RenderWindow& window) const;

    sf::Texture* getFrontTexture();
    sf::Texture* getBackTexture();
    sf::Sprite& getSprite();

};
