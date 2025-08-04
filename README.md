# 🧠 Jogo da Memória em Rede (Terminal/WebSocket)

## 👥 Grupo

* Arthus Lopes Fernandes
* Gustavo Araújo Menezes Castro
* João Paulo Lacerda Leão de Oliveira

---

## Instruções de Execução

### 1. Clone o repositório

```bash
git clone <https://github.com/JoaoLace/memory_game-SFML>
cd <memory_game-SFML>
```

### 2. Instale as dependências

#### Fedora / RHEL / CentOS

```bash
sudo dnf install gcc-c++ make boost-devel asio-devel websocketpp-devel
```

#### Debian / Ubuntu / Linux Mint / Pop!\_OS

```bash
sudo apt update
sudo apt install g++ make libboost-all-dev libasio-dev libwebsocketpp-dev
```

#### Arch Linux / Manjaro / EndeavourOS

```bash
sudo pacman -Syu
sudo pacman -S gcc make boost asio websocketpp
```

#### Para se conectar ao servidor

```bash
npm install wscat
```
> **Este projeto não utiliza SFML, Python ou bibliotecas gráfica diferente da primera versão.**

---

### 3. Compile o projeto

Com o Makefile:

```bash
make
```

Ou manualmente:

```bash
g++ -std=c++17 -pthread -o memory_game memory_game.cpp
```

---

### 4. Execute o servidor

```bash
./memory_game
```

---

### 5. Conecte ao servidor com um cliente WebSocket

Você pode usar o `wscat`:

```bash
wscat -c ws://localhost:9003
```

---

## IP e Porta Utilizados

* **IP**: `localhost` (127.0.0.1)
* **Porta**: `9003`

---

## 📃 Protocolo de Aplicação

A comunicação entre servidor e clientes é feita via **WebSockets** com mensagens de texto baseadas em comandos simples.

### Comandos

* `PING` — Testa a conexão
* `HELP` — Lista os comandos disponíveis
* `STATE` — Mostra o estado atual do jogo
* `BOARD` — Exibe o tabuleiro
* `REVEAL x,y` — Revela a carta na posição (x,y)
* `RESET` — Reinicia a partida (Novo)
* `QUIT` — Desconecta do servidor

---

### Exemplos de comandos

```
REVEAL 0,1
STATE
BOARD
RESET
```

---

### Exemplos de respostas do servidor

```
REVEAL_OK 5 GATO
MATCH 5 13 GATO Score: 3 x 2
MISMATCH 6 10 Próximo: Jogador2
GAME_OVER Vencedor: Jogador1 (6 pontos)
```

---

## Sessões de Teste

### Sessão entre dois jogadores

```
Jogador1: REVEAL 0,0
Servidor: REVEAL_OK 0 CASA
Jogador1: REVEAL 1,0
Servidor: MATCH 0 1 CASA Score: 1 x 0

Jogador1: REVEAL 2,0
Servidor: REVEAL_OK 2 LIVRO
Jogador1: REVEAL 3,0
Servidor: MISMATCH 2 3 Próximo: Jogador2

Jogador2: REVEAL 1,1
...
```

### Sessão de teste com terminal

```
wscat -c ws://localhost:9003
> HELP
< Comandos disponíveis: PING, HELP, STATE, BOARD, REVEAL x,y, RESET, QUIT
> REVEAL 2,2
< REVEAL_OK 10 MESA
```

---

## Funcionalidades Implementadas

* Jogo da memória 4x4 via terminal
* Conexão multiplayer com até dois jogadores
* Turnos alternados e pontuação automática
* Atualizações em tempo real para todos os clientes
* Fim de jogo com vencedor declarado
* Suporte a comando de reinício (`RESET`)

---

> 🎓 Feito com carinho para o professor Eduardo Habib Miranda!
