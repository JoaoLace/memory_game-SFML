#include "../lib/game.hpp"
#include <thread>

int main()
{
    srand(time(0));
 
    // Servidor WebSocket separado para testes
    std::thread t([](){
        servidor s;
        s.run();
    });

    game Game;
    Game.run();
    return 0;
}
