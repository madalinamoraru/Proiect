#include "Enemy.h"
#include <cmath>

// Implementare Enemy (Baza)
Enemy::Enemy(float hp, const sf::Vector2f& size) : Entity(hp, size) {}

// Implementare Dummy
Dummy::Dummy(const sf::Vector2f& pos) : Enemy(50.f, {40.f, 40.f}) {
    body.setFillColor(sf::Color(100, 100, 100)); // Gri
    body.setPosition(pos);
}

void Dummy::update(sf::Time dt, const sf::Vector2u& windowSize) {
    // Nu face nimic, e tinta
}

void Dummy::draw(sf::RenderWindow& window) const {
    window.draw(body);
}

Entity* Dummy::clone() const {
    return new Dummy(*this);
}

// Implementare Shade
Shade::Shade(const sf::Vector2f& pos) : Enemy(80.f, {30.f, 30.f}) {
    body.setFillColor(sf::Color::Red);
    body.setPosition(pos);
}

void Shade::move(const sf::Vector2f& targetPos, sf::Time dt) {
    sf::Vector2f currentPos = body.getPosition();
    sf::Vector2f direction = targetPos - currentPos;
    
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length != 0) {
        direction /= length; // Normalizare
        body.move(direction * chaseSpeed * dt.asSeconds());
    }
}

void Shade::update(sf::Time dt, const sf::Vector2u& windowSize) {
    // Shade trebuie mutat din Game (pentru a avea acces la pozitia Player-ului)
}

void Shade::draw(sf::RenderWindow& window) const {
    window.draw(body);
}

Entity* Shade::clone() const {
    return new Shade(*this);
}