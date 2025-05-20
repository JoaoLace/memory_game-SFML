#pragma once

// Includes
#include "gui.hpp"
#include "card.hpp"
#include "random.hpp"
#include <ctime>

enum vez { jogador1, jogador2 };

// Constantes globais
const sf::VideoMode windowSize = sf::VideoMode(800, 600);
const std::string title = "Jogo da Memoria - Habib";

class game {
public:
    game();
    ~game();

    void run(); // Loop principal

private:
    // Janela e estado do jogo
    sf::RenderWindow* window;
    bool running;
    bool started;
    bool gameOver = false;
    bool isSpeedrunMode;

    // Recursos visuais e fontes
    sf::Font font;
    sf::Sprite backgroundPelado;
    sf::Texture backgroundPeladoTexture;

    // Interface e lógica inicial
    gui initialScreen;
    int tema;
    std::string temaFunc();

    // Cartas
    std::vector<card> cards;
    card* firstFlipped = nullptr;
    card* secondFlipped = nullptr;
    sf::Clock flipClock;
    bool waitingToUnflip = false;

    // Placar e pontuação
    std::vector<sf::Text> placar;
    int j1Pontos, j2Pontos;
    int jAtual;     // 0 ou 1
    int jPontos;    // pontos do jogador caso esteja em modo de speedrun
    sf::Text endText;

    // Speedrun
    sf::Clock speedrunClock;
    sf::Text speedrunText;

    // Inicialização
    void init();
    void initWindow();
    void initFont();
    void initVariables();
    void initCards();
    void initPlacar();

    // Atualização
    void update();
    void updateEvents();
    void updatePlacar();

    // Renderização
    void render();
    void renderCards();
    void renderPlacar();

    // Lógica do jogo
    void setTema(int novoTema);
    void startGame();
    void endGame();
    void morte(); // kkkkkkkkkkkk

    void handleCardClick();
    void checkForMatch();
    bool checkGameOver();
};
