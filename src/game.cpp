#include "../lib/game.hpp"

// construtor
game::game(){
    init();
}

// destrutor
game::~game(){
    delete window;
}

// inits
void game::init(){
    initVariables();
    initWindow();
    initFont();
    initCards();
    initPlacar();
    initialScreen.init(font);
}

void game::initCards() {
    cards.clear(); 

    std::string folder = temaFunc();
    std::string folderLower = "";
    std::string filePath = "assets/";
    std::string strTema = "";
    

    switch (tema)
    {
        case memes:
            filePath += "memes/";
            break;
        case aleatorio:
            strTema += my_random::tema();
            filePath += "random/";
            my_random::run(strTema);
            break;
        case escolha:
            std::cout << "\n\n\nDigite o tema: " << std::flush;
            std:: getline(std::cin, strTema);
            filePath += "random/";
            my_random::run(strTema);
            break;
        case speedrun:
            std::cout << "Seu destino foi decretado...";
            filePath += "memes/";
            break;
        default:
            break;
    }


    for (char x : folder) folderLower += tolower(x);


    std::string backImagePath = "assets/backImage.png";

    std::cout << filePath << std::flush;

    int cols = 5;  
    int rows = 4;  
    float cardWidth = 100;
    float cardHeight = 100; 
    float padding = 10;     

    // Gera os pares de cartas
    for (int i = 1; i <= 10; i++) {
        std::string frontImagePath = filePath + std::to_string(i) + ".jpg";
        cards.emplace_back(i, frontImagePath, backImagePath, sf::Vector2f(0, 0));
        cards.emplace_back(i, frontImagePath, backImagePath, sf::Vector2f(0, 0));
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);

    // Espaço reservado para o placar
    float topOffset = 100.0f;

    // Tamanho total do grid
    float totalWidth = cols * cardWidth + (cols - 1) * padding;
    float totalHeight = rows * cardHeight + (rows - 1) * padding;

    // Centraliza horizontalmente baseado na largura da janela
    float xStart = (window->getSize().x - totalWidth) / 2.0f;
    float yStart = topOffset;

    int col = 0, row = 0;
    for (size_t i = 0; i < cards.size(); i++) {
        float xPos = xStart + col * (cardWidth + padding); 
        float yPos = yStart + row * (cardHeight + padding); 

        cards[i].getSprite().setPosition(sf::Vector2f(xPos, yPos));

        col++;
        if (col >= cols) {
            col = 0;
            row++;
        }

        // Ajusta escala das texturas
        // if (cards[i].getFrontTexture() != nullptr) {
        //     cards[i].getSprite().setScale(cardWidth / cards[i].getFrontTexture()->getSize().x,
        //                                   cardHeight / cards[i].getFrontTexture()->getSize().y);
        // }

        // if (cards[i].getBackTexture() != nullptr) {
        //     cards[i].getSprite().setScale(cardWidth / cards[i].getBackTexture()->getSize().x,
        //                                   cardHeight / cards[i].getBackTexture()->getSize().y);
        // }
    }
}


void game::initFont(){
    if (!font.loadFromFile("assets/JetBrainsMono-Bold-Italic.ttf")){
        std::cout << "Deu merda na fonte, doidao";
    }

    speedrunText.setFont(font);
    speedrunText.setCharacterSize(24);
    speedrunText.setFillColor(sf::Color::Red);
    speedrunText.setPosition(10.f, 50.f); 

}

void game::initPlacar(){
    placar.push_back(createText(font,20,{10,10},sf::Color::White,"Jogador 1: "));
    placar.push_back(createText(font,20,{500,10},sf::Color::White,"Jogador 2: "));

}

void game::initWindow(){
    window = new sf::RenderWindow(windowSize, title, sf::Style::Titlebar);
    window->setFramerateLimit(30);
    window->setPosition(sf::Vector2i(500,100));
}

void game::initVariables(){
    running = true;
    started = false;
    tema = nulo;
    jAtual = vez::jogador1;
    jPontos = 0;
    j1Pontos = 0;
    j2Pontos = 0;
    isSpeedrunMode = false;

    if (!backgroundPeladoTexture.loadFromFile("assets/backgroundPelado.png")) {
        std::cerr << "Erro ao carregar o fundo 'backgroundPelado.png'\n";
    }
    backgroundPelado.setTexture(backgroundPeladoTexture);


    sf::Vector2u windowSize = sf::Vector2u(800, 600); 

    sf::Vector2u textureSize = backgroundPeladoTexture.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    backgroundPelado.setScale(scaleX, scaleY);

    backgroundPelado.setPosition(0, 0);
}

// Updates
void game::update(){
    updateEvents();
    if (gameOver) return;
    if (!started) {
        initialScreen.update(*window);
    } else {
        if (isSpeedrunMode) {
            float t = speedrunClock.getElapsedTime().asSeconds();
            std::ostringstream ss;
            ss << "Tempo: " << std::fixed << std::setprecision(2) << t << "s";
            speedrunText.setString(ss.str());

            if (t >= 10){
                morte();
                gameOver = true;
            }
        }
        updatePlacar();
        checkForMatch();
        handleCardClick();
        if (checkGameOver()) endGame();
    }
}
 

void game::updateEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        
        if (event.type == sf::Event::Closed) {
            window->close();
            running = false;
        }

    if (!started) {
        int temaSelecionado = initialScreen.testClick(*window, event);
        if (temaSelecionado != temas::nulo) {  
            tema = static_cast<temas>(temaSelecionado);  
            startGame();
        }
    }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                std::cout << "Você pressionou 'Escape'";
                window->close();
                running = false;
            }
        }
    }

    if (gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        started = false;
        gameOver = false;
        j1Pontos = 0;
        j2Pontos = 0;
        tema = temas::nulo;
        placar.clear();
        init();
    }

}

void game::updatePlacar(){
    std::string str = "Jogador 1: " + std::to_string(j1Pontos);
    placar.at(jogador1).setString(str);

    str = "Jogador 2: " + std::to_string(j2Pontos);
    placar.at(jogador2).setString(str);

}


// Render
void game::render(){
    window->clear();
    if (!started){
        initialScreen.render(*window);
    } else {
        window->draw(backgroundPelado);
        renderCards();
        if (!isSpeedrunMode) renderPlacar();
        if (isSpeedrunMode) window->draw(speedrunText);
        if (gameOver) window->draw(endText);
    }
    window->display();
}



void game::renderCards(){
    for (card x : cards) x.render(*window);
}

void game::run(){
    while (running){
        update();
        render();
    }
}

void game::renderPlacar(){
    for (sf::Text x : placar) window->draw(x);
}

std::string game::temaFunc() {
    switch (tema) {
        case temas::nulo:
            return "";
        case temas::memes:
            return "memes";
        case temas::aleatorio:
            return "aleatorio";
        case temas::speedrun:
            return "speedrun";
        case temas::escolha:
            return "escolha";
        default:
            return "";
    }
}


void game::setTema(int novoTema){
    if (novoTema >= temas::nulo && novoTema <= temas::speedrun)
        tema = static_cast<temas>(novoTema);
}


void game::startGame(){
    started = true;
    initCards();
    gameOver = false;
    if (tema == temas::speedrun) {
        isSpeedrunMode = true;
        speedrunClock.restart();
    } else {
        isSpeedrunMode = false;
    }
}


void game::handleCardClick() {
    if (waitingToUnflip) return;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

        for (card& c : cards) {
            if (c.isClicked(mousePos) && !c.getIsFlipped() && !c.getIsMatched()) {
                c.flip();

                if (!firstFlipped) {
                    firstFlipped = &c;
                } else if (!secondFlipped && &c != firstFlipped) {
                    secondFlipped = &c;
                    flipClock.restart();
                    waitingToUnflip = true;
                }

                break;
            }
        }
    }
}
void game::checkForMatch() {
    if (waitingToUnflip && flipClock.getElapsedTime().asSeconds() >= 1.0f) {
        if (firstFlipped->getId() == secondFlipped->getId()) {
            firstFlipped->markMatched();
            secondFlipped->markMatched();

            if (jAtual == jogador1) j1Pontos++;
            else if (jAtual == jogador2) j2Pontos++;

        } else {
            firstFlipped->unflip();
            secondFlipped->unflip();

            if (jAtual == jogador1)
                jAtual = jogador2;
            else
                jAtual = jogador1;
        }

        firstFlipped = nullptr;
        secondFlipped = nullptr;
        waitingToUnflip = false;
    }
}

void game::endGame(){
    gameOver = true;

    std::string resultado;
    if (j1Pontos > j2Pontos) resultado = "Jogador 1 venceu!";
    else if (j2Pontos > j1Pontos) resultado = "Jogador 2 venceu!";
    else resultado = "Empate!";

    endText.setFont(font);
    endText.setCharacterSize(30);
    endText.setFillColor(sf::Color::Yellow);
    endText.setString("Fim de Jogo!\n" + resultado + "\nPressione Enter para reiniciar");
    
    // Centraliza o texto na tela
    sf::FloatRect bounds = endText.getLocalBounds();
    endText.setOrigin(bounds.width / 2, bounds.height / 2);
    endText.setPosition(window->getSize().x / 2.0f, window->getSize().y / 2.0f);   
}

bool game::checkGameOver(){
    if (j1Pontos + j2Pontos == 10) return true;

    return false;
}

void game::morte(){
    std::string com = "./src/morte.sh";
    system(com.c_str());
}