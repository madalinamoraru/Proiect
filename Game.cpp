#include "Game.h"
#include <iostream>

// Initializarea atributului static
const unsigned Game::DEFAULT_WIDTH;

// --- Implementare Game ---

Game::Game(unsigned w, unsigned h, const std::string& title) 
    : window(sf::VideoMode(w, h), title), 
      background(20, 20, 24), fps(60) 
{
    // Utilizare cu sens: throw in constructor
    if (w < 100 || h < 100) {
        throw InitializationError("Window dimensions too small.");
    }
    window.setFramerateLimit(fps);
    spawnInitialEnemies();
}

void Game::spawnInitialEnemies() {
    // Adaugam pointeri la clasa de baza in container
    enemies.push_back(std::make_unique<Dummy>(sf::Vector2f(100.f, 100.f)));
    enemies.push_back(std::make_unique<Shade>(sf::Vector2f(600.f, 200.f)));
    enemies.push_back(std::make_unique<Shade>(sf::Vector2f(200.f, 500.f)));
    
    std::cout << "Total entities spawned: " << Entity::getEntityCount() << "\n";
}

void Game::checkCollisions() {
    // Iterați prin inamici
    for (auto it_e = enemies.begin(); it_e != enemies.end(); ) {
        bool enemyDestroyed = false;
        Entity* enemyPtr = it_e->get();
        
        // Polimorfism: inamicul se misca
        if (auto shadePtr = dynamic_cast<Shade*>(enemyPtr)) {
            shadePtr->move(player.bounds().getPosition(), sf::seconds(0.016f)); // dt mic
        }

        // Coliziune cu proiectile (simplificat)
        auto& bullets = player.getBullets(); // (accesam direct, presupunand ca bullets e public temporar)
        for (auto it_b = bullets.begin(); it_b != bullets.end(); ) {
            if (enemyPtr->bounds().intersects(it_b->bounds())) {
                enemyPtr->takeDamage(20.f); 
                it_b = bullets.erase(it_b); // Stergem proiectilul
                
                if (enemyPtr->getHP() <= 0) {
                    it_e = enemies.erase(it_e); // Stergem inamicul
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
    // (Restul logicii input-ului)
    input.up    = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    input.down  = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    input.left  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    input.right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    input.fire  = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
}

void Game::update(sf::Time dt) {
    player.update(dt, window.getSize());
    
    // Apelul functiilor virtuale prin pointeri la clasa de baza
    for (const auto& entityPtr : enemies) {
        entityPtr->update(dt, window.getSize());
    }
    
    checkCollisions(); // Contine dynamic_cast si polimorfism
}

void Game::render() {
    window.clear(background);
    player.draw(window);
    
    // Apelul functiilor virtuale prin pointeri la clasa de baza
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
        player.handleInput(input, dt);
        update(dt);
        render();
    }
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    // Afisam starea jucatorului la finalul jocului
    os << "\n--- Game Summary ---\n";
    os << "Player HP: " << game.player.getHP() << "\n";

    // Presupunand ca Player::operator<< este si el implementat si friend cu Player
    // Daca ai implementat Player::operator<<, foloseste:
    os << game.player;

    os << "Total entitati ramase: " << Entity::getEntityCount() << "\n";
    os << "--------------------\n";

    return os;
}