#pragma once

// Includes
#include "gui.hpp"
#include "card.hpp"
#include "random.hpp"
#include <ctime>

enum vez {jogador1, jogador2};

// Definindo algumas merdas
const sf::VideoMode windowSize = sf::VideoMode(800,600);
const std::string title = "Jogo da Memoria - Habib";    



class game {
    public:
        game();
        ~game();

        void run();

    private:
        // variaveis
        sf::RenderWindow *window;
        bool running;
        bool started;
        int tema; 
        sf::Font font;
        gui initialScreen;
        std::vector<card> cards;
        sf::Sprite backgroundPelado;
        sf::Texture backgroundPeladoTexture;
        std::vector<sf::Text> placar; 
        int j1Pontos,j2Pontos,jAtual,jPontos;
        sf::Text endText;
        bool gameOver = false;
        sf::Clock speedrunClock;
        sf::Text speedrunText;
        bool isSpeedrunMode;


        // Metodos
        void init();
        void initWindow();
        void initFont();
        void initVariables();
        void initCards();
        void initPlacar();

        void update();
        void updateEvents();
        void updatePlacar();

        void render();
        void renderCards();
        void renderPlacar();

        void setTema(int novoTema);
        void startGame();
        void endGame();
        void morte();

        void handleCardClick();
        void checkForMatch();
        bool checkGameOver();

        card* firstFlipped = nullptr;
        card* secondFlipped = nullptr;
        sf::Clock flipClock;
        bool waitingToUnflip = false;


        std::string temaFunc();
};

