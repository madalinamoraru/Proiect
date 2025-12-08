#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

// Clasa de Baza abstracta pentru Inamici
class Enemy : public Entity {
public:
    Enemy(float hp, const sf::Vector2f& size);
    virtual ~Enemy() override = default;

    // Functie virtuala pura specifica inamicilor
    virtual void move(const sf::Vector2f& targetPos, sf::Time dt) = 0;
};

// Inamic Derivat 1: Dummy (Tinta statica)
class Dummy : public Enemy {
public:
    Dummy(const sf::Vector2f& pos);
    virtual ~Dummy() override = default;

    // Suprascriem functiile virtuale
    virtual void update(sf::Time dt, const sf::Vector2u& windowSize) override;
    virtual void draw(sf::RenderWindow& window) const override;
    virtual Entity* clone() const override;

    // Suprascriem functia move: nu se misca
    virtual void move(const sf::Vector2f& targetPos, sf::Time dt) override {}
};

// Inamic Derivat 2: Shade (Urmareste Jucatorul)
class Shade : public Enemy {
private:
    float chaseSpeed = 100.f;
public:
    Shade(const sf::Vector2f& pos);
    virtual ~Shade() override = default;

    // Suprascriem functiile virtuale
    virtual void update(sf::Time dt, const sf::Vector2u& windowSize) override;
    virtual void draw(sf::RenderWindow& window) const override;
    virtual Entity* clone() const override;

    // Implementam logica de urmarire
    virtual void move(const sf::Vector2f& targetPos, sf::Time dt) override;
};

#endif // ENEMY_H