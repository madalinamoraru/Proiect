#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory> // std::unique_ptr
#include "Player.h"
#include "Enemy.h"
#include "GameException.h" // Pentru throw in constructor

class Game {
private:
    sf::RenderWindow window;
    sf::Color background;
    unsigned fps;

    std::vector<std::unique_ptr<Entity>> enemies;

    static const unsigned DEFAULT_WIDTH = 800;
    float spawnTimer;
    float spawnInterval;
    unsigned int maxEnemies;

public:
    Game(unsigned w = DEFAULT_WIDTH, unsigned h = 600, const std::string& title = "Shadow's Dojo");

    friend std::ostream& operator<<(std::ostream& os, const Game& game);

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void run();

    void spawnInitialEnemies();
    void checkCollisions();

private:
    void processEvents(InputState& input);
    void update(sf::Time dt);
    void render();
};

#endif // GAME_H