#include "EnemyFactory.h"

EnemyFactory::EnemyFactory()
{

}

EnemyFactory::~EnemyFactory()
{
    
}

Enemy* EnemyFactory::makeEnemy(Player* player, sf::Vector2f size, int type)
{
    Enemy* enemy = new Enemy(player);

    switch(type)
    {
        case 1: // Basic melee
            enemy->setWeapon(0.6f, 0.1f, 0.1f, 40, 60, 15);
            enemy->setAnimation("greenEnemy.json");
            enemy->setMaxSpeed(300.0f, 800.0f);
            enemy->setAIDistances(150, 50);
            enemy->setRangedMode(false);
            enemy->setMaxHealth(90);
            break;
        case 2: // Basic ranged
            enemy->setRangedWeapon(2.0f, 0.1f, 10);
            enemy->setAnimation("yellowEnemy.json");
            enemy->setMaxSpeed(500.0f, 800.0f);
            enemy->setAIDistances(650, 600);
            enemy->setRangedMode(true);
            enemy->setMaxHealth(50);
            break;
        default: 
            enemy->setWeapon(0.6f, 0.1f, 0.1f, 40, 60, 15);
            enemy->setAnimation("greenEnemy.json");
            enemy->setMaxSpeed(300.0f, 800.0f);
            enemy->setAIDistances(150, 50);
            enemy->setRangedMode(false);
            enemy->setMaxHealth(90);
            break;
    }

    enemy->setSize(size);
    
    return enemy;
}
