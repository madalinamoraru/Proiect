#include "Game.h"
#include "Player.h"
#include "EnemyFactory.h"
#include "Enemy.h"
#include <iostream>
#include "GameException.h"


Game::Game(unsigned w, unsigned h, const std::string& title)
    : window(sf::VideoMode(w, h), title),
      background(20, 20, 24), fps(60),
      spawnTimer(0.f),
      spawnInterval(2.0f), // Spawn every 2 seconds
      maxEnemies(10)
{

    if (w < 100 || h < 100) {
        throw InitializationError("Window dimensions too small.");
    }
    window.setFramerateLimit(fps);
    spawnInitialEnemies();
}

void Game::spawnInitialEnemies() {


    for (int i = 0; i < 5; ++i) {
        auto newEnemy = EnemyFactory::createRandomEnemy(window.getSize());

        if (newEnemy) {
            enemies.push_back(std::move(newEnemy));
        }
    }

    std::cout << "Initial spawn complete.\n";
    std::cout << "Total entities spawned: " << Entity::getEntityCount() << "\n";
}

void Game::checkCollisions() {
    Player& player = Player::getInstance();

    for (auto it_e = enemies.begin(); it_e != enemies.end(); ) {
        bool enemyDestroyed = false;
        Entity* enemyPtr = it_e->get();

        if (auto shadePtr = dynamic_cast<Shade*>(enemyPtr)) {
            shadePtr->move(player.bounds().getPosition(), sf::seconds(0.016f));
        }

        auto& bullets = player.getBullets();

        for (auto it_b = bullets.begin(); it_b != bullets.end(); ) {
            if (enemyPtr->bounds().intersects(it_b->bounds())) {
                enemyPtr->takeDamage(20.f);
                it_b = bullets.erase(it_b);

                if (enemyPtr->getHP() <= 0) {
                    it_e = enemies.erase(it_e);
                    enemyDestroyed = true;
                    break;
                }
            } else {
                ++it_b;
            }
        }

        if (!enemyDestroyed) {
            ++it_e;
        }
    }
}

void Game::processEvents(InputState& input) {
    sf::Event e;
    while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed) window.close();
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
            window.close();
    }

    input.up    = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    input.down  = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    input.left  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    input.right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    input.fire  = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
}

void Game::update(sf::Time dt) {
    Player::getInstance().update(dt, window.getSize());

    spawnTimer += dt.asSeconds();

    if (spawnTimer >= spawnInterval && enemies.size() < maxEnemies) {

        spawnTimer = 0.f;

        auto newEnemy = EnemyFactory::createRandomEnemy(window.getSize());
        if (newEnemy) {
            enemies.push_back(std::move(newEnemy));
            std::cout << "Reinforcement arrived! Total: " << enemies.size() << "\n";
        }


    }


    for (const auto& entityPtr : enemies) {
        entityPtr->update(dt, window.getSize());
    }

    checkCollisions();
}

void Game::render() {
    window.clear(background);

    Player::getInstance().draw(window);

    for (const auto& entityPtr : enemies) {
        entityPtr->draw(window);
    }

    window.display();
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        InputState input{};
        processEvents(input);
        sf::Time dt = clock.restart();

        Player::getInstance().handleInput(input, dt);

        update(dt);
        render();
    }
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "\n--- Game Summary ---\n";

    Player& player = Player::getInstance();

    os << "Player HP: " << player.getHP() << "\n";

    os << player;

    os << "Total entities remaining: " << Entity::getEntityCount() << "\n";
    os << "--------------------\n";

    return os;
}