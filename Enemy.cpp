#include "Enemy.h"
#include <cmath>


Enemy::Enemy(float hp, const sf::Vector2f& size) : Entity(hp, size) {}


Dummy::Dummy(const sf::Vector2f& pos) : Enemy(50.f, {40.f, 40.f}) {
    body.setFillColor(sf::Color(100, 100, 100)); // Gri
    body.setPosition(pos);
}

void Dummy::update(sf::Time dt, const sf::Vector2u& windowSize) {

}

void Dummy::draw(sf::RenderWindow& window) const {
    window.draw(body);
}

Entity* Dummy::clone() const {
    return new Dummy(*this);
}


Shade::Shade(const sf::Vector2f& pos) : Enemy(80.f, {30.f, 30.f}) {
    body.setFillColor(sf::Color::Red);
    body.setPosition(pos);
}

void Shade::move(const sf::Vector2f& targetPos, sf::Time dt) {
    sf::Vector2f currentPos = body.getPosition();
    sf::Vector2f direction = targetPos - currentPos;
    
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length != 0) {
        direction /= length;
        body.move(direction * chaseSpeed * dt.asSeconds());
    }
}

void Shade::update(sf::Time dt, const sf::Vector2u& windowSize) {

}

void Shade::draw(sf::RenderWindow& window) const {
    window.draw(body);
}

Entity* Shade::clone() const {
    return new Shade(*this);
}