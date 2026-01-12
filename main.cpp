#include "Game.h"
#include <iostream>
#include <exception>


int main() {
    Game game;
    try {
        Game actual_game(800, 600, "Shadow's Dojo POO");

        actual_game.run();

        std::cout << actual_game;

    }
    catch (const InitializationError& e) {
        std::cerr << "Exceptie de Initializare (Critica): " << e.what() << "\n";
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Exceptie Standard (STD): " << e.what() << "\n";
        return 3;
    }

    return 0;
}