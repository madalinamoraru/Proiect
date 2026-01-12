#include "EnemyFactory.h"
#include "Enemy.h"
#include "Player.h"
#include <random>
#include <cmath>

float getRandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

int getRandomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

std::unique_ptr<Enemy> EnemyFactory::createEnemy(EnemyType type, const sf::Vector2u& windowSize) {

    sf::Vector2f pos = getSafeSpawnPosition(windowSize);

    switch (type) {
        case EnemyType::Dummy:
            return std::make_unique<Dummy>(pos);
        case EnemyType::Shade:
            return std::make_unique<Shade>(pos);
        default:
            return nullptr;
    }
}

std::unique_ptr<Enemy> EnemyFactory::createRandomEnemy(const sf::Vector2u& windowSize) {

    int randomType = getRandomInt(0, 1);
    EnemyType type = (randomType == 0) ? EnemyType::Dummy : EnemyType::Shade;

    return createEnemy(type, windowSize);
}

sf::Vector2f EnemyFactory::getSafeSpawnPosition(const sf::Vector2u& windowSize) {
    sf::Vector2f pos;
    bool safe = false;
    int attempts = 0;
    const float minDistance = 300.f;

    sf::Vector2f playerPos = Player::getInstance().bounds().getPosition();

    while (!safe && attempts < 20) {

        pos.x = getRandomFloat(50.f, windowSize.x - 50.f);
        pos.y = getRandomFloat(50.f, windowSize.y - 50.f);

        float dx = pos.x - playerPos.x;
        float dy = pos.y - playerPos.y;
        float distance = std::sqrt(dx*dx + dy*dy);

        if (distance > minDistance) {
            safe = true;
        }
        attempts++;
    }


    return pos;
}