#include "Entity.h"
#include <algorithm> // Pentru std::swap

// Initializarea atributului static
int Entity::entityCount = 0; 

// --- Implementare Projectile (Copy and Swap) ---

void swap(Projectile& first, Projectile& second) noexcept {
    // Folosim std::swap pentru atribute
    using std::swap;
    swap(first.shape, second.shape);
    swap(first.velocity, second.velocity);
}

Projectile::Projectile(const sf::Vector2f& pos, const sf::Vector2f& vel) : velocity(vel) {
    shape.setSize({12.f, 12.f});
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(pos);
}

// Constructor de Copiere
Projectile::Projectile(const Projectile& other) : 
    shape(other.shape), 
    velocity(other.velocity) 
{}

// Operator de Atribuire (foloseste Copy-and-Swap)
Projectile& Projectile::operator=(Projectile other) noexcept {
    swap(*this, other); 
    return *this;
}

void Projectile::update(sf::Time dt) {
    shape.move(velocity * dt.asSeconds());
}

void Projectile::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

// --- Implementare Entity (Baza) ---

Entity::Entity(float initialHP, const sf::Vector2f& size) : currentHP(initialHP) {
    body.setSize(size);
    entityCount++; // Incrementam atributul static
}

Entity::~Entity() {
    entityCount--; // Decrementam atributul static
}

void Entity::takeDamage(float damage) {
    if (damage > 0) {
        currentHP -= damage;
    }
}