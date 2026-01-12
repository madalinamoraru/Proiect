#include "Player.h"
#include <algorithm>
#include <iostream>

using std::max;
using std::min;
using std::remove_if;

// 1. Constructor remains the same, but it is now private (defined in Header)
Player::Player() : Entity(100.f, {60.f, 40.f}) {
    body.setFillColor(sf::Color(100, 200, 255));
    body.setPosition(370.f, 520.f);
}

// 2. REMOVED clone() method.
// A Singleton implies there is only ONE. Cloning creates a second one.
// If your Entity class forces you to have clone(), you must restructure your hierarchy.

void Player::handleInput(const InputState& input, sf::Time dt) {
    sf::Vector2f movement{0.f, 0.f};

    if (input.up)    movement.y -= speed;
    if (input.down)  movement.y += speed;
    if (input.left)  { movement.x -= speed; lastDirection = Facing::Left; }
    if (input.right) { movement.x += speed; lastDirection = Facing::Right; }

    body.move(movement * dt.asSeconds());

    timeSinceShot += dt.asSeconds();
    if (input.fire && timeSinceShot >= fireCooldown) {
        shoot();
        timeSinceShot = 0.f;
    }
}

void Player::update(sf::Time dt, const sf::Vector2u& windowSize) {
    auto pos = body.getPosition();
    auto size = body.getSize();
    pos.x = max(0.f, min(pos.x, (float)windowSize.x - size.x));
    pos.y = max(0.f, min(pos.y, (float)windowSize.y - size.y));
    body.setPosition(pos);

    for (auto& b : bullets) b.update(dt);

    bullets.erase(
        remove_if(bullets.begin(), bullets.end(),
            [&](const Projectile& p){
                auto box = p.bounds();
                return box.left + box.width < 0.f || box.left > windowSize.x;
            }),
        bullets.end()
    );
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(body);
    for (const auto& b : bullets) b.draw(window);
}

void Player::shoot() {
    auto box = body.getGlobalBounds();
    const float bulletSize = 12.f;
    float y = box.top + box.height * 0.5f - bulletSize * 0.5f;

    if (lastDirection == Facing::Left) {
        float x = box.left - bulletSize; 
        bullets.emplace_back(sf::Vector2f{x, y}, sf::Vector2f{-600.f, 0.f});
    } else {
        float x = box.left + box.width; 
        bullets.emplace_back(sf::Vector2f{x, y}, sf::Vector2f{+600.f, 0.f});
    }
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    sf::Vector2f pos = player.body.getPosition();
    os << "Player Position: (" << pos.x << ", " << pos.y << "), HP: " << player.getHP() << "\n";
    return os;
}