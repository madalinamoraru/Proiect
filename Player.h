#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "InputState.h"
#include <vector>

class Player : public Entity {

    float speed = 260.f;
    std::vector<Projectile> bullets;
    float fireCooldown = 0.15f;
    float timeSinceShot = 0.f;

    enum class Facing { Left, Right };
    Facing lastDirection = Facing::Right;

    Player();

public:

    static Player& getInstance() {
        static Player instance;
        return instance;
    }


    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    virtual ~Player() override = default;

    std::vector<Projectile>& getBullets() { return bullets; }

    virtual void update(sf::Time dt, const sf::Vector2u& windowSize) override;
    virtual void draw(sf::RenderWindow& window) const override;

    void handleInput(const InputState& input, sf::Time dt);

    friend std::ostream& operator<<(std::ostream& os, const Player& player);

private:
    void shoot();
};

#endif // PLAYER_H