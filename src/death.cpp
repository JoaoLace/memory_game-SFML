#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

std::string titles[5] = {"MORTE", "FUTILIDADE", "ABANDONO", "DOR", "AGONIA"};

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Carregar som
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("assets/hihi/aa.mp3")) {
        std::cerr << "Erro ao carregar o áudio!" << std::endl;
        return 1;
    }

    sf::Sound som;
    som.setBuffer(buffer);
    som.play();

    // Modo de vídeo da tela
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    int titleIndex = rand() % 5;

    // Criar janela em tela cheia
    sf::RenderWindow window(desktop, titles[titleIndex], sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // Imagem aleatória
    int imageIndex = rand() % 5 + 1;
    std::string imagePath = "assets/hihi/" + std::to_string(imageIndex) + ".jpg";

    sf::Texture texture;
    if (!texture.loadFromFile(imagePath)) {
        std::cerr << "Erro ao carregar imagem: " << imagePath << std::endl;
        return 1;
    }

    sf::Sprite sprite(texture);

    // Redimensionar imagem para preencher a tela
    sf::Vector2u textureSize = texture.getSize();
    sf::Vector2u windowSize = window.getSize();
    sprite.setScale(
        static_cast<float>(windowSize.x) / textureSize.x,
        static_cast<float>(windowSize.y) / textureSize.y
    );

    // Loop principal
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
