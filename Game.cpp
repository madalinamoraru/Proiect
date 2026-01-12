#include "Game.h"
#include "Player.h" // Essential: We need to see the Player class to call getInstance()
#include <iostream>
#include "GameException.h"

// Initialize static attribute
const unsigned Game::DEFAULT_WIDTH;

// --- Game Implementation ---

Game::Game(unsigned w, unsigned h, const std::string& title)
    : window(sf::VideoMode(w, h), title),
      background(20, 20, 24), fps(60)
{
    // Throw in constructor
    if (w < 100 || h < 100) {
        throw InitializationError("Window dimensions too small.");
    }
    window.setFramerateLimit(fps);
    spawnInitialEnemies();
}

void Game::spawnInitialEnemies() {
    enemies.push_back(std::make_unique<Dummy>(sf::Vector2f(100.f, 100.f)));
    enemies.push_back(std::make_unique<Shade>(sf::Vector2f(600.f, 200.f)));
    enemies.push_back(std::make_unique<Shade>(sf::Vector2f(200.f, 500.f)));

    std::cout << "Total entities spawned: " << Entity::getEntityCount() << "\n";
}

void Game::checkCollisions() {
    // 1. Create a local reference to the Singleton
    Player& player = Player::getInstance();

    // Iterate through enemies
    for (auto it_e = enemies.begin(); it_e != enemies.end(); ) {
        bool enemyDestroyed = false;
        Entity* enemyPtr = it_e->get();

        // Polymorphism: Enemy movement logic
        if (auto shadePtr = dynamic_cast<Shade*>(enemyPtr)) {
            // "player" here refers to the singleton reference defined above
            shadePtr->move(player.bounds().getPosition(), sf::seconds(0.016f));
        }

        // Collision with bullets
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
    // 2. Access Singleton directly
    Player::getInstance().update(dt, window.getSize());

    // Call virtual functions via base class pointers
    for (const auto& entityPtr : enemies) {
        entityPtr->update(dt, window.getSize());
    }

    checkCollisions();
}

void Game::render() {
    window.clear(background);

    // 3. Access Singleton directly
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

        // 4. Access Singleton directly
        Player::getInstance().handleInput(input, dt);

        update(dt);
        render();
    }
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "\n--- Game Summary ---\n";

    // 5. Access Singleton directly (game.player no longer exists)
    // We get the global instance instead
    Player& player = Player::getInstance();

    os << "Player HP: " << player.getHP() << "\n";

    // Assuming Player has a friend operator<<
    os << player;

    os << "Total entities remaining: " << Entity::getEntityCount() << "\n";
    os << "--------------------\n";

    return os;
}