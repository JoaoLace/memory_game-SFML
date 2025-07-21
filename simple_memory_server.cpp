#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <thread>
#include <chrono>

using namespace std;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

typedef websocketpp::server<websocketpp::config::asio> server;

class MemoryGame {
private:
    vector<string> words;
    vector<bool> revealed;
    vector<string> players;
    int currentPlayer;
    int firstCard;
    int secondCard;
    bool waitingForSecondCard;
    int score1, score2;

public:
    map<websocketpp::connection_hdl, string, std::owner_less<websocketpp::connection_hdl>> connections;

    MemoryGame() {
        words = {"CASA", "CARRO", "GATO", "CACHORRO", "LIVRO", "MESA", "PORTA", "JANELA"};
        vector<string> temp = words;
        words.insert(words.end(), temp.begin(), temp.end());

        random_device rd;
        mt19937 g(rd());
        shuffle(words.begin(), words.end(), g);

        revealed = vector<bool>(words.size(), false);
        currentPlayer = 0;
        firstCard = -1;
        secondCard = -1;
        waitingForSecondCard = false;
        score1 = 0;
        score2 = 0;
    }

    void addPlayer(const string& name) {
        if (players.size() < 2) {
            players.push_back(name);
            cout << "Jogador " << name << " conectado!" << endl;
        }
    }

    void addConnection(websocketpp::connection_hdl hdl, const string& name) {
        connections[hdl] = name;
        addPlayer(name);
    }

    void removeConnection(websocketpp::connection_hdl hdl) {
        auto it = connections.find(hdl);
        if (it != connections.end()) {
            string name = it->second;
            connections.erase(it);
            auto playerIt = find(players.begin(), players.end(), name);
            if (playerIt != players.end()) {
                players.erase(playerIt);
            }
            cout << "Jogador " << name << " desconectado!" << endl;
        }
    }

    string getGameState() {
        string state = "STATE ";
        state += to_string(players.size()) + "/2 ";

        if (players.empty()) {
            state += "Aguardando jogadores...";
        } else if (players.size() == 1) {
            state += "Aguardando segundo jogador...";
        } else {
            state += "Jogando - ";
            state += "Jogador " + to_string(currentPlayer + 1) + " (" + players[currentPlayer] + ") ";
            state += "Score: " + to_string(score1) + " x " + to_string(score2);
        }
        return state;
    }

    string getBoard() {
        string board = "BOARD\n\t0\t1\t2\t3\n";
        for (int y = 0; y < 4; y++) {
            board += to_string(y) + "\t";
            for (int x = 0; x < 4; x++) {
                int i = y * 4 + x;
                board += (revealed[i] ? words[i] : "***") + "\t";
            }
            board += "\n";
        }
        return board;
    }

    string revealCard(int index) {
        if (index < 0 || index >= (int)words.size()) return "ERROR Posição inválida";
        if (revealed[index]) return "ERROR Carta já revelada";
        if (waitingForSecondCard && index == firstCard) return "ERROR Carta já selecionada";

        revealed[index] = true;

        if (!waitingForSecondCard) {
            firstCard = index;
            waitingForSecondCard = true;
            return "REVEAL_OK " + to_string(index) + " " + words[index];
        } else {
            secondCard = index;
            waitingForSecondCard = false;

            if (words[firstCard] == words[secondCard]) {
                if (currentPlayer == 0) score1++;
                else score2++;
                string result = "MATCH " + to_string(firstCard) + " " + to_string(secondCard) + " " + words[firstCard];
                result += " Score: " + to_string(score1) + " x " + to_string(score2);
                return result;
            } else {
                revealed[firstCard] = false;
                revealed[secondCard] = false;
                currentPlayer = (currentPlayer + 1) % 2;
                string result = "MISMATCH " + to_string(firstCard) + " " + to_string(secondCard);
                result += " Próximo: " + players[currentPlayer];
                return result;
            }
        }
    }

    bool isGameOver() {
        return all_of(revealed.begin(), revealed.end(), [](bool b) { return b; });
    }

    string getWinner() {
        if (score1 > score2) return "Vencedor: " + players[0] + " (" + to_string(score1) + " pontos)";
        else if (score2 > score1) return "Vencedor: " + players[1] + " (" + to_string(score2) + " pontos)";
        else return "Empate! Ambos com " + to_string(score1) + " pontos";
    }

    void reset() {
        revealed = vector<bool>(words.size(), false);
        currentPlayer = 0;
        firstCard = -1;
        secondCard = -1;
        waitingForSecondCard = false;
        score1 = 0;
        score2 = 0;

        random_device rd;
        mt19937 g(rd());
        shuffle(words.begin(), words.end(), g);
    }
};

MemoryGame game;
server ws_server;

void on_open(server* s, websocketpp::connection_hdl hdl) {
    cout << "Nova conexão!" << endl;
    static int playerCount = 0;
    string playerName = "Jogador" + to_string(++playerCount);
    game.addConnection(hdl, playerName);
    try {
        s->send(hdl, game.getGameState(), websocketpp::frame::opcode::text);
        s->send(hdl, game.getBoard(), websocketpp::frame::opcode::text);
    } catch (const websocketpp::exception& e) {
        cout << "Erro ao enviar mensagem: " << e.what() << endl;
    }
}

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
    string message = msg->get_payload();
    cout << "Mensagem recebida: " << message << endl;

    if (message == "PING") {
        s->send(hdl, "PONG", websocketpp::frame::opcode::text);
    }
    else if (message == "HELP") {
        string help = "Comandos disponíveis:\n";
        help += "PING - Testa conexão\n";
        help += "HELP - Lista comandos\n";
        help += "STATE - Estado do jogo\n";
        help += "BOARD - Mostra tabuleiro\n";
        help += "REVEAL x,y - Revela carta (ex: REVEAL 1,2)\n";
        help += "RESET - Reinicia jogo\n";
        help += "QUIT - Desconecta";
        s->send(hdl, help, websocketpp::frame::opcode::text);
    }
    else if (message == "STATE") {
        s->send(hdl, game.getGameState(), websocketpp::frame::opcode::text);
    }
    else if (message == "BOARD") {
        s->send(hdl, game.getBoard(), websocketpp::frame::opcode::text);
    }
    else if (message.substr(0, 7) == "REVEAL ") {
        try {
            string coords = message.substr(7);
            size_t comma = coords.find(',');
            if (comma == string::npos) throw invalid_argument("Faltando vírgula");

            int x = stoi(coords.substr(0, comma));
            int y = stoi(coords.substr(comma + 1));
            if (x < 0 || x > 3 || y < 0 || y > 3) throw out_of_range("Coordenadas inválidas");

            int pos = y * 4 + x;
            string result = game.revealCard(pos);
            s->send(hdl, result, websocketpp::frame::opcode::text);

            // Atualiza todos os jogadores
            string state = game.getGameState();
            string board = game.getBoard();
            for (auto& conn : game.connections) {
                try {
                    s->send(conn.first, state, websocketpp::frame::opcode::text);
                    s->send(conn.first, board, websocketpp::frame::opcode::text);
                } catch (...) {}
            }

            // Fim de jogo
            if (game.isGameOver()) {
                string winner = game.getWinner();
                for (auto& conn : game.connections) {
                    try {
                        s->send(conn.first, "GAME_OVER " + winner, websocketpp::frame::opcode::text);
                    } catch (...) {}
                }
            }

        } catch (const exception& e) {
            s->send(hdl, string("ERROR Formato inválido: ") + e.what(), websocketpp::frame::opcode::text);
        }
    }
    else if (message == "RESET") {
        game.reset();
        string state = game.getGameState();
        string board = game.getBoard();
        for (auto& conn : game.connections) {
            try {
                s->send(conn.first, "RESET_OK", websocketpp::frame::opcode::text);
                s->send(conn.first, state, websocketpp::frame::opcode::text);
                s->send(conn.first, board, websocketpp::frame::opcode::text);
            } catch (...) {}
        }
    }
    else if (message == "QUIT") {
        s->close(hdl, websocketpp::close::status::normal, "Desconectado pelo usuário");
    }
    else {
        s->send(hdl, "ERROR Comando desconhecido: " + message, websocketpp::frame::opcode::text);
    }
}

void on_close(server* s, websocketpp::connection_hdl hdl) {
    game.removeConnection(hdl);
    cout << "Conexão fechada!" << endl;
}

int main() {
    cout << "🎮 Servidor do Jogo da Memória" << endl;
    cout << "==============================" << endl;

    ws_server.set_access_channels(websocketpp::log::alevel::none);
    ws_server.set_error_channels(websocketpp::log::elevel::fatal);
    ws_server.init_asio();

    ws_server.set_open_handler(bind(&on_open, &ws_server, ::_1));
    ws_server.set_message_handler(bind(&on_message, &ws_server, ::_1, ::_2));
    ws_server.set_close_handler(bind(&on_close, &ws_server, ::_1));

    try {
        ws_server.listen(9003);
        ws_server.start_accept();

        cout << "✅ Servidor iniciado na porta 9003" << endl;
        cout << "🌐 Conecte via: wscat -c ws://localhost:9003" << endl;
        cout << "📋 Comandos: PING, HELP, STATE, BOARD, REVEAL x,y, RESET, QUIT" << endl;
        cout << "🎯 Grid: coordenadas de 0 a 3 (ex: REVEAL 1,2)" << endl;

        ws_server.run();
    } catch (const websocketpp::exception& e) {
        cout << "❌ Erro no servidor: " << e.what() << endl;
        return 1;
    }

    return 0;
}
