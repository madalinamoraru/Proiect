#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "InputState.h"
#include <vector>

class Player : public Entity {
private:
    float speed = 260.f;
    std::vector<Projectile> bullets;
    float fireCooldown = 0.15f;
    float timeSinceShot = 0.f;

    enum class Facing { Left, Right };
    Facing lastDirection = Facing::Right;

public:
    Player();
    std::vector<Projectile>& getBullets() { return bullets; }
    // Suprascriem:
    virtual ~Player() override = default;
    virtual void update(sf::Time dt, const sf::Vector2u& windowSize) override;
    virtual void draw(sf::RenderWindow& window) const override;
    virtual Entity* clone() const override; // Constructor virtual

    // Suprascriem cc/op= (folosim atribute simple, default e OK dar le declaram explicit)
    Player(const Player&) = default;
    Player& operator=(const Player&) = default;

    // Metoda specifica
    void handleInput(const InputState& input, sf::Time dt);

    // Friend pentru supraincarcarea operatorului <<
    friend std::ostream& operator<<(std::ostream& os, const Player& player);

private:
    void shoot();
};

#endif // PLAYER_H