#ifndef EnemyFactoryFACTORY_H
#define EnemyFactory_H

#include "Engine.h"
#include "Enemy.h"

class EnemyFactory {
private:

public:
    EnemyFactory();
    ~EnemyFactory();

    static Enemy* makeEnemy(Player* player, sf::Vector2f size, int type, float statsFactor=1.0f);
};

#endif
