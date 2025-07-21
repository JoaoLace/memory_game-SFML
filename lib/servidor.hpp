#pragma once
#include <sstream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <set>
#include <mutex>
#include <unordered_set>
#include <functional>

// Forward declaration
class game;

class servidor {
public:
	servidor();
	~servidor();

	void run();
	void setGameCallback(std::function<void(int, int)> callback);
	void setGameStateCallback(std::function<std::string()> callback);
	void broadcastGameState();

private:
	std::pair<int, int> ultimas_revelacoes[2][2]; // até 2 revelações por jogador
	int num_reveladas[2] = {0, 0};

	typedef websocketpp::server<websocketpp::config::asio> server;
	
	std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> clients;
	websocketpp::connection_hdl jogadores[2];
	int total_conectados = 0;
	int vez_do_jogador = 0;
	std::mutex clients_mutex;
	
	std::unordered_set<std::string> comandos_validos = {"ping", "pong", "help", "reveal", "quit", "state"};
	
	// Callbacks para comunicação com o jogo
	std::function<void(int, int)> gameCardCallback;
	std::function<std::string()> gameStateCallback;
	
	bool is_valid_command(const std::string& cmd);
	bool is_free_command(const std::string& cmd);
	
	void on_open(server* s, websocketpp::connection_hdl hdl);
	void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg);
	void sendToAll(server* s, const std::string& message);
};

