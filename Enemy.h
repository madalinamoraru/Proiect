#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy(float hp, const sf::Vector2f& size);
    virtual ~Enemy() override = default;

    virtual void move(const sf::Vector2f& targetPos, sf::Time dt) = 0;
};

class Dummy : public Enemy {
public:
    Dummy(const sf::Vector2f& pos);
    virtual ~Dummy() override = default;

    virtual void update(sf::Time dt, const sf::Vector2u& windowSize) override;
    virtual void draw(sf::RenderWindow& window) const override;
    virtual Entity* clone() const override;

    virtual void move(const sf::Vector2f& targetPos, sf::Time dt) override {}
};

class Shade : public Enemy {
private:
    float chaseSpeed = 100.f;
public:
    Shade(const sf::Vector2f& pos);
    virtual ~Shade() override = default;

    virtual void update(sf::Time dt, const sf::Vector2u& windowSize) override;
    virtual void draw(sf::RenderWindow& window) const override;
    virtual Entity* clone() const override;

    virtual void move(const sf::Vector2f& targetPos, sf::Time dt) override;
};

#endif // ENEMY_H