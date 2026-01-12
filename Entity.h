#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <iostream>

// Functia swap pentru copy-and-swap (declarata inainte)
class Projectile;
void swap(Projectile& first, Projectile& second) noexcept;

// Projectile (Clasa mica legata de Entitati)
class Projectile {
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;

public:
    Projectile(const sf::Vector2f& pos, const sf::Vector2f& vel);
    friend void swap(Projectile& first, Projectile& second) noexcept;

    // Suprascris cc/op= folosind Copy and Swap
    Projectile(const Projectile& other); // Copy constructor
    Projectile& operator=(Projectile other) noexcept; // Operator de atribuire

    // Destructor implicit (default)
    ~Projectile() = default;

    // Functii
    void update(sf::Time dt);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect bounds() const { return shape.getGlobalBounds(); }
};


// Clasa de Baza Abstracta
class Entity {
private:
    float currentHP;
    // Atribut static
    static int entityCount;

protected:
    sf::RectangleShape body;

public:
    Entity(float initialHP, const sf::Vector2f& size);

    // Destructor virtual (obligatoriu in clasa de baza)
    virtual ~Entity();

    // Functii virtuale pure (Entity devine clasa abstracta)
    virtual void update(sf::Time dt, const sf::Vector2u& windowSize) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
    // Constructor virtual (clone)
    virtual Entity* clone() const { return nullptr; }

    // Functii virtuale
    virtual void takeDamage(float damage);

    // Functii si atribute statice
    static int getEntityCount() { return entityCount; }

    // Metode simple
    sf::FloatRect bounds() const { return body.getGlobalBounds(); }
    float getHP() const { return currentHP; }
};

#endif // ENTITY_H