#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;

namespace my_random {
    // Limpa as imagens da pasta assets/random/
    void clear();

    // Chama feeder.py com o tema 
    void feed(std::string input);

    // Muda o tamanho das imagens para 100x100
    void resize();

    // Chama todas as funções
    void run(std::string input);

    // Retorna um tema aleatório do arquivo assets/temas.txt
    std::string tema();
}
