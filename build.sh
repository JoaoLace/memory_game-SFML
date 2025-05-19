#!/bin/bash
set -e

echo "Verificando dependências..."

# Verfificando SFML
if ! ldconfig -p | grep -q sfml; then
    echo "ERROR: SFML não encontrado. Instale-o antes de continuar."
    exit 1
fi

# Verificar Python
if ! command -v python3 &> /dev/null; then
    echo "ERROR: Python3 não encontrado."
    exit 1
fi

# Verificar módulos Python
if ! python3 -c "import requests, duckduckgo_search" &> /dev/null; then
    echo "ERROR: Módulos Python necessários não instalados. Executando: pip install requests duckduckgo_search"
    pip install requests duckduckgo_search
fi

# Verificar mogrify
if ! command -v mogrify &> /dev/null; then
    echo "ERROR: 'mogrify' não encontrado. Instale o ImageMagick."
    exit 1
fi

#Compilando um arquivo totalmente seguro
g++ src/death.cpp -o death -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

echo "✅ Todas as dependências estão OK."

echo "Compilando o jogo com makefile..."
make

echo "Rodando o jogo..."
./sfml-app

echo "🚀 Jogo compilado com sucesso em ./sfml-app"
