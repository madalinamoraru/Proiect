#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <iostream>


class Projectile;
void swap(Projectile& first, Projectile& second) noexcept;


class Projectile {
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;

public:
    Projectile(const sf::Vector2f& pos, const sf::Vector2f& vel);
    friend void swap(Projectile& first, Projectile& second) noexcept;


    Projectile(const Projectile& other);
    Projectile& operator=(Projectile other) noexcept;


    ~Projectile() = default;


    void update(sf::Time dt);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect bounds() const { return shape.getGlobalBounds(); }
};



class Entity {
private:
    float currentHP;
    static int entityCount;

protected:
    sf::RectangleShape body;

public:
    Entity(float initialHP, const sf::Vector2f& size);


    virtual ~Entity();


    virtual void update(sf::Time dt, const sf::Vector2u& windowSize) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;

    virtual Entity* clone() const { return nullptr; }


    virtual void takeDamage(float damage);


    static int getEntityCount() { return entityCount; }


    sf::FloatRect bounds() const { return body.getGlobalBounds(); }
    float getHP() const { return currentHP; }
};

#endif // ENTITY_H