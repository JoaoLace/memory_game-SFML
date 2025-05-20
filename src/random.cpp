#include "../lib/random.hpp"

// Mesma coisa do random.hpp...

void my_random::clear(){
    std::string path = "assets/random"; 

    for (const auto& entry : fs::directory_iterator(path)) {
        std::string arquivo = path + entry.path().filename().string();

        try {if (fs::remove(entry.path())) {
            std::cout << entry.path().filename().string() << " foi removido" << std::endl;
         } else {
            std::cout << "ERROR";
         }
        }
        catch(const std::filesystem::filesystem_error& err) {
            std::cout << "filesystem error: " << err.what() << '\n';
        }
    }
}

void my_random::feed(std::string input){
    std::string command = "python3 feeder.py " + input;
    system(command.c_str());

}

void my_random::resize() {
    std::string command = "mogrify -path assets/random -resize 100x100\\! assets/random/*.jpg";
    system(command.c_str());
}


void my_random::run(std::string input) {
    clear();
    feed(input);
    resize();
}

std::string my_random::tema(){
    std::ifstream arqTemas("/home/joao/dev/c/memory_game/assets/temas.txt");

    if(!arqTemas.is_open()){
        std::cerr << "Erro no arquivo de temas" << std::endl;
    }

    std::string strTema;

    int x = rand() % 100;
    int i = 0;

    while(std::getline(arqTemas, strTema)){
        if (x == i) break;
        else i++;
    }

    arqTemas.close();

    return strTema;

}