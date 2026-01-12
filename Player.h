#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "InputState.h"
#include <vector>
#include <iostream> // Needed for std::ostream

class Player : public Entity {
private:
    // 1. Member variables (Keep these as they were)
    float speed = 260.f;
    std::vector<Projectile> bullets;
    float fireCooldown = 0.15f;
    float timeSinceShot = 0.f;

    enum class Facing { Left, Right };
    Facing lastDirection = Facing::Right;

    // 2. PRIVATE CONSTRUCTOR
    // We move this here so no one outside can write "Player p;"
    Player();

public:
    // 3. STATIC ACCESS METHOD (The Singleton "Getter")
    static Player& getInstance() {
        static Player instance; // Created only once
        return instance;
    }

    // 4. DELETE COPYING AND ASSIGNMENT
    // We strictly forbid copying the singleton
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    // 5. REMOVE CLONE
    // A Singleton is unique, so it cannot be cloned.
    // (Note: If 'Entity' has clone() as pure virtual, you might need to adjust Entity)
    // virtual Entity* clone() const override;  <-- REMOVED

    // 6. Public Methods
    virtual ~Player() override = default;

    std::vector<Projectile>& getBullets() { return bullets; }

    virtual void update(sf::Time dt, const sf::Vector2u& windowSize) override;
    virtual void draw(sf::RenderWindow& window) const override;

    // Specific method for input
    void handleInput(const InputState& input, sf::Time dt);

    // Friend for operator overloading
    friend std::ostream& operator<<(std::ostream& os, const Player& player);

private:
    void shoot();
};

#endif // PLAYER_H