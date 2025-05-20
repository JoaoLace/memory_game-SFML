## Jogo da Memória para o Eduardo Habib!!

### Grupo:
- Arthus Lopes Fernandes
- Gustavo Araújo Menezes Castro
- João Paulo Lacerda Leão de Oliveira

### Execução (vai dar trabalho)

1. Clone o repositório.
2. De permissão para ```build.sh``` com:

```bash
chmod +x build.sh
```

3. O arquivo vai te informar o que você deve instalar caso você não tenha as dependencies necessárias. Mas, de qualquer forma, aqui estão elas:

- ```GCC/G++``` para compilar os arquivos cpp;
- ```Makefile``` pra compilar o projeto;
- ```SFML```  (biblioteca gráfica);
- ```Pyhton3``` para rodar um arquivo .py necessário pra algumas funções do jogo;
- Bibliotecas ```requests``` e ```duckduckgo_search``` (serão instaladas automaticamente);
- ```ImageMagick```, aplicação do Linux para redimensionar imagens por comandos do cmd.
  

4. Caso você não tenha as dependências necessárias, aqui vai um mini tutorial: (não sei qual distro tu usa, então vou falar como baixa nas mais famosas)

- **Fedora / RHEL / CentOS**

```bash
sudo dnf install gcc-c++ make SFML SFML-devel python3 ImageMagick
```


- Debian / Ubuntu / Linux Mint / Pop!_OS

``` shell
sudo apt update
sudo apt install g++ make libsfml-dev python3 python3-pip imagemagic
```

- Arch Linux / Manjaro / EndeavourOS
```bash
sudo pacman -Syu
sudo pacman -S gcc make sfml python python-pip imagemagick
```

5. Agora, com todas as dependências instaladas, ou você da ```./build.sh``` ou ```make``` + ```./sfml-app``` 

### Ferramentas 

Bom, eu já meio que falei quais são kk, mas aqui está denovo:

- ```GCC/G++``` para compilar os arquivos cpp;
- ```Makefile``` pra compilar o projeto;
- ```SFML```  (biblioteca gráfica);
- ```Pyhton3``` para rodar um arquivo .py necessário pra algumas funções do jogo;
- Bibliotecas ```requests``` e ```duckduckgo_search``` (serão instaladas automaticamente);
- ```ImageMagick```, aplicação do Linux para redimensionar imagens por comandos do cmd.

### Funcionalidades

É um jogo da memoria padrão, porém com algumas coisas especiais…
Quando o programa rodar, você poderá escolher entre 4 opções:
- Memes (carrega o tema de memes);
- Aleatório (carrega um tema aleatório);
- Escolha (abre um prompt no cmd pra você digitar o tema que quiser);
- Speedrun (usa o tema de memes, mas também registra o tempo que você está demorando pra finalizar o jogo).

### Estrutura

TEM ARQUIVO COM BORRA.

Sendo sincero, eu me empolguei um pouco mais do que deveria, mas enfim. As pasta são bastante auto explicativas, mas aqui está um descrição delas:

- assets/ 
	Carrega todos os arquivos necessários pra mostrar a tela pro jogador, como background, fonte, imagens para os temas etc.

- lib/
	Pasta de biblioteca com os arquivos .hpp e definições de classes, eu faço dessa forma pra não precisar me preocupar com a ordem em que as funções são definidas.

- src/
	Todos os arquivos .cpp necessários pra rodar o jogo.

- feeder.py
	Usa uma biblioteca para baixar imagens da internet.

