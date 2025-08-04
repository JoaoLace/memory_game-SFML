# Jogo da Memória em Rede - README

## Grupo

* Arthus Lopes Fernandes
* Gustavo Araújo Menezes Castro
* João Paulo Lacerda Leão de Oliveira

## Instruções de Execução

### 1. Clone o repositório

```bash
git clone <url-do-repositorio>
cd <nome-da-pasta>
```

### 2. Dê permissão de execução ao script de build

```bash
chmod +x build.sh
```

### 3. Instale as dependências

#### Fedora / RHEL / CentOS

```bash
sudo dnf install gcc-c++ make SFML SFML-devel python3 ImageMagick
```

#### Debian / Ubuntu / Linux Mint / Pop!\_OS

```bash
sudo apt update
sudo apt install g++ make libsfml-dev python3 python3-pip imagemagick
```

#### Arch Linux / Manjaro / EndeavourOS

```bash
sudo pacman -Syu
sudo pacman -S gcc make sfml python python-pip imagemagick
```

> As bibliotecas Python `requests` e `duckduckgo_search` serão instaladas automaticamente.

### 4. Compile o projeto

```bash
make -f 
```

### 5. Execute o servidor

```bash
./memory_server
```

### 6. Conecte ao servidor com o cliente WebSocket

```bash
wscat -c ws://localhost:9003
```

## 📃 Protocolo de Aplicação

A comunicação entre servidor e clientes é feita via WebSockets. O protocolo é textual e baseado em comandos simples:

### Comandos

* `PING` - Testa a conexão
* `HELP` - Lista os comandos disponíveis
* `STATE` - Retorna o estado atual do jogo
* `BOARD` - Retorna o tabuleiro atual
* `REVEAL x,y` - Revela a carta na posição (x,y) do tabuleiro (coordenadas de 0 a 3)
* `RESET` - Reinicia o jogo (Novo em relação a implementação.)
* `QUIT` - Desconecta o cliente

### Exemplo de Comandos

```
REVEAL 1,2
STATE
BOARD
```

### Exemplo de Respostas

```
REVEAL_OK 6 CACHORRO
MATCH 6 14 CACHORRO Score: 2 x 1
MISMATCH 2 5 Próximo: Jogador2
GAME_OVER Vencedor: Jogador1 (5 pontos)
```

## IP e Porta Utilizados

* **IP**: `localhost` (127.0.0.1)
* **Porta**: `9003`

## Sessões de Teste

### Sessão entre dois jogadores

```
Jogador1: REVEAL 0,0
Servidor: REVEAL_OK 0 CASA
Jogador1: REVEAL 1,0
Servidor: MATCH 0 1 CASA Score: 1 x 0

Jogador1: REVEAL 2,0
Servidor: REVEAL_OK 2 MESA
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
< REVEAL_OK 10 LIVRO
```

## Funcionalidades Implementadas

* Tabuleiro 4x4 com pares de palavras embaralhadas
* Dois jogadores conectados via WebSocket
* Controle de turnos e pontuação
* Respostas automáticas do servidor com atualizações para todos os clientes
* Reinício de partida
* Reconhecimento de fim de jogo com declaração de vencedor



Feito com carinho para o professor Eduardo Habib Miranda! 
