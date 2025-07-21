#include "servidor.hpp"

servidor::servidor() {}

servidor::~servidor() {}

void servidor::setGameCallback(std::function<void(int, int)> callback) {
    gameCardCallback = callback;
}

void servidor::setGameStateCallback(std::function<std::string()> callback) {
    gameStateCallback = callback;
}

void servidor::broadcastGameState() {
    if (gameStateCallback) {
        std::string state = gameStateCallback();
        // Enviar estado para todos os clientes conectados
        // Implementação será feita quando necessário
    }
}

void servidor::sendToAll(server* s, const std::string& message) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto& client : clients) {
        if (!client.expired()) {
            try {
                s->send(client, message, websocketpp::frame::opcode::text);
            } catch (const websocketpp::exception& e) {
                std::cerr << "Erro ao enviar mensagem: " << e.what() << std::endl;
            }
        }
    }
}

bool servidor::is_valid_command(const std::string& cmd) {
    return comandos_validos.find(cmd) != comandos_validos.end();
}

// Comandos que qualquer um pode enviar a qualquer momento
bool servidor::is_free_command(const std::string& cmd) {
    return cmd == "ping" || cmd == "help" || cmd == "state";
}

void servidor::on_open(server* s, websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(clients_mutex);

    if (total_conectados >= 2) {
        s->close(hdl, websocketpp::close::status::normal, "Servidor cheio");
        return;
    }

    clients.insert(hdl);
    jogadores[total_conectados] = hdl;
    total_conectados++;

    std::string msg = "Jogador " + std::to_string(total_conectados) + " conectado!";
    std::cout << msg << std::endl;

    try {
        s->send(hdl, msg, websocketpp::frame::opcode::text);
        
        // Se é o segundo jogador, inicia o jogo
        if (total_conectados == 2) {
            sendToAll(s, "GAME_START");
            sendToAll(s, "TURN Player1");
        }
    } catch (const websocketpp::exception& e) {
        std::cerr << "Erro ao enviar mensagem de boas-vindas: " << e.what() << std::endl;
    }
}

void servidor::on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::string payload = msg->get_payload();
    std::cout << "Mensagem recebida: " << payload << std::endl;

    std::lock_guard<std::mutex> lock(clients_mutex);

    // Comando malformado
    if (payload.empty()) {
        s->send(hdl, "ERROR Malformed command", msg->get_opcode());
        return;
    }

    // Comando base
    std::istringstream ss(payload);
    std::string cmd;
    ss >> cmd;

    // Normaliza para uppercase
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

    // Ping/pong
    if (cmd == "PING") {
        s->send(hdl, "PONG", msg->get_opcode());
        return;
    }

    if (cmd == "HELP") {
        s->send(hdl, "Comandos: REVEAL x,y | QUIT | PING | STATE", msg->get_opcode());
        return;
    }

    if (cmd == "STATE") {
        if (gameStateCallback) {
            std::string state = gameStateCallback();
            s->send(hdl, "GAME_STATE " + state, msg->get_opcode());
        } else {
            s->send(hdl, "GAME_STATE No game state available", msg->get_opcode());
        }
        return;
    }

    if (cmd == "QUIT") {
        s->close(hdl, websocketpp::close::status::normal, "Saindo do jogo.");
        return;
    }

    // Somente jogador da vez pode enviar comandos de jogo
    if (hdl.lock() != jogadores[vez_do_jogador].lock()) {
        s->send(hdl, "ERROR Não é sua vez", msg->get_opcode());
        return;
    }

    if (cmd == "REVEAL") {
        int x, y;
        char comma;
        if (!(ss >> x >> comma >> y) || comma != ',') {
            s->send(hdl, "REVEAL_FAIL Invalid format", msg->get_opcode());
            return;
        }

        if (x < 0 || x >= 5 || y < 0 || y >= 4) {
            s->send(hdl, "REVEAL_FAIL Invalid coordinates", msg->get_opcode());
            return;
        }

        // Chama o callback do jogo para revelar a carta
        if (gameCardCallback) {
            gameCardCallback(x, y);
        }

        // Notifica ambos sobre a carta revelada
        for (int i = 0; i < 2; i++) {
            if (!jogadores[i].expired()) {
                s->send(jogadores[i], "REVEAL_OK " + std::to_string(x) + "," + std::to_string(y), msg->get_opcode());
            }
        }

        // Armazena revelação
        ultimas_revelacoes[vez_do_jogador][num_reveladas[vez_do_jogador]] = {x, y};
        num_reveladas[vez_do_jogador]++;

        if (num_reveladas[vez_do_jogador] == 2) {
            // Aguarda um pouco para verificar se há match
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            
            // Simula verificação de par (usando ID fictício)
            auto [x1, y1] = ultimas_revelacoes[vez_do_jogador][0];
            auto [x2, y2] = ultimas_revelacoes[vez_do_jogador][1];
            bool par = (x1 + y1) % 2 == (x2 + y2) % 2; // Simulação simples

            std::string resultado = par ? "MATCH" : "MISMATCH";
            std::string mensagem = resultado + " " + 
                std::to_string(x1) + "," + std::to_string(y1) + "," + 
                std::to_string(x2) + "," + std::to_string(y2);

            for (int i = 0; i < 2; i++) {
                if (!jogadores[i].expired()) {
                    s->send(jogadores[i], mensagem, msg->get_opcode());
                }
            }

            if (!par) {
                // Muda a vez apenas se errar
                vez_do_jogador = 1 - vez_do_jogador;
            }

            // Limpa revelações
            num_reveladas[0] = 0;
            num_reveladas[1] = 0;

            // Notifica turno
            for (int i = 0; i < 2; i++) {
                if (!jogadores[i].expired()) {
                    std::string tmsg = (i == vez_do_jogador)
                        ? "TURN Player" + std::to_string(i + 1)
                        : "Aguarde sua vez.";
                    s->send(jogadores[i], tmsg, msg->get_opcode());
                }
            }
        }

        return;
    }

    // Comando desconhecido
    s->send(hdl, "ERROR Unknown command", msg->get_opcode());
}

void servidor::run() {
    server ws_server;

    ws_server.init_asio();

    ws_server.set_open_handler([this, &ws_server](websocketpp::connection_hdl hdl) {
        on_open(&ws_server, hdl);
    });

    ws_server.set_message_handler([this, &ws_server](websocketpp::connection_hdl hdl, server::message_ptr msg) {
        on_message(&ws_server, hdl, msg);
    });

    try {
        ws_server.listen(9003);
        ws_server.start_accept();

        std::cout << "Servidor WebSocket com revezamento iniciado na porta 9003..." << std::endl;

        ws_server.run();
    } catch (const websocketpp::exception& e) {
        std::cerr << "Erro no servidor: " << e.what() << std::endl;
        std::cerr << "Porta 9003 pode estar em uso. Tente novamente." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erro inesperado: " << e.what() << std::endl;
    }
}
