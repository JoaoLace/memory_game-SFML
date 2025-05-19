#include "../lib/gui.hpp"

bool isMouseOverText(const sf::Text& text, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    return text.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

int gui::testText(sf::RenderWindow &window){
    for (int i = op1; i <= op4; i++){ 
        if (isMouseOverText(texts.at(i), window)) {
            return i;
        }
    }
    return -1; 
}


void gui::init(const sf::Font& font){
    texts.push_back(createText(font, 40, {200,40},sf::Color::Blue,""));
    texts.push_back(createText(font, 25, {280,100}, sf::Color::Blue,"Escolha um tema:"));
    texts.push_back(createText(font, 25, {330, 155}, sf::Color::Blue, "Memes"));
    texts.push_back(createText(font, 25, {330, 210}, sf::Color::Blue, "Aleatorio"));
    texts.push_back(createText(font, 25, {330, 265}, sf::Color::Blue, "Escolha"));
    texts.push_back(createText(font, 25, {330, 320}, sf::Color::Blue, "Speedrun"));

    backgroundTexture.loadFromFile("assets/background.png");
    background.setTexture(backgroundTexture);

    sf::Vector2u windowSize = sf::Vector2u(800, 600); 

    sf::Vector2u textureSize = backgroundTexture.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    background.setScale(scaleX, scaleY);

    background.setPosition(0, 0);


}

void gui::render(sf::RenderTarget &window){
    window.draw(background);

    for (sf::Text text : texts){
        window.draw(text);
    }
}

int gui::testClick(sf::RenderWindow &window, sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Button::Left) {
            int index = testText(window); 
            if (index != -1) {
                std::string str = texts.at(index).getString();
                std::cout << "Você clicou em: " << str << std::endl;

                if (str == "Memes") return temas::memes;
                if (str == "Aleatorio") return temas::aleatorio;
                if (str == "Escolha")  return temas::escolha;
                if (str == "Speedrun") return temas::speedrun;
            }
        }
    }
    return temas::nulo;  
}





void gui::update(sf::RenderWindow &window) {
    updateColor(window);
}

void gui::updateColor(sf::RenderWindow &window){
    if (testText(window)!= -1) texts.at(testText(window)).setFillColor(sf::Color::White);
    else{
        for (int i = op1; i < op3 + 1; i++) texts.at(i).setFillColor(sf::Color::Blue);
    }
}

sf::Text createText(const sf::Font& font, float size, sf::Vector2f pos, sf::Color color, std::string text){
    sf::Text temp;   
    temp.setFont(font);
    temp.setCharacterSize(size);
    temp.setPosition(pos);
    temp.setFillColor(color);
    temp.setString(text);
    return temp;
}


