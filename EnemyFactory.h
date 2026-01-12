#ifndef ENEMYFACTORY_H
#define ENEMYFACTORY_H

#include <memory>
#include <SFML/Graphics.hpp>
#include "Enemy.h"


enum class EnemyType {
    Dummy,
    Shade
};

class EnemyFactory {
public:
    static std::unique_ptr<Enemy> createEnemy(EnemyType type, const sf::Vector2u& windowSize);
    static std::unique_ptr<Enemy> createRandomEnemy(const sf::Vector2u& windowSize);

private:
    static sf::Vector2f getSafeSpawnPosition(const sf::Vector2u& windowSize);
};

#endif // ENEMYFACTORY_H