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
            enemy->setWeapon(0.6f, 0.1f, 0.1f, 40, 60, 15, {1500.0f,0.0f}, HEAVY_ATTACK);
            enemy->setAnimation("greenEnemy.json");
            enemy->setMaxSpeed(300.0f, 800.0f);
            enemy->setAIDistances(150, 50);
            enemy->setRangedMode(false);
            enemy->setMaxHealth(90);
            enemy->setResistances(0.0f, 0.0f, 0.0f);
            break;
        case 2: // Basic ranged
            enemy->setRangedWeapon(2.0f, 0.1f, 10);
            enemy->setAnimation("yellowEnemy.json");
            enemy->setMaxSpeed(500.0f, 800.0f);
            enemy->setAIDistances(650, 600);
            enemy->setRangedMode(true);
            enemy->setMaxHealth(50);
            enemy->setResistances(0.0f, 0.0f, 0.0f);
            break;
        case 10: // Breakable wall
            enemy->setWeapon(0.1f, 0.0f, 0.1f, 0, 0, 0);
            enemy->setAnimation("yellowEnemy.json");
            enemy->setMaxSpeed(0.0f, 800.0f);
            enemy->setAIDistances(0, 0);
            enemy->setRangedMode(false);
            enemy->setMaxHealth(1);
            enemy->setResistances(1.0f, 0.0f, 1.0f);
            enemy->setDoPatrol(false);
            break;
        case 11: // Platform enemy
            enemy->setWeapon(0.1f, 0.0f, 0.1f, 0, 0, 0);
            enemy->setAnimation("yellowEnemy.json");
            enemy->setMaxSpeed(500.0f, 800.0f);
            enemy->setAIDistances(50*1.2, 0);
            enemy->setRangedMode(false);
            enemy->setMaxHealth(1000);
            enemy->setResistances(1.0f, 1.0f, 1.0f);
            enemy->setDoPatrol(false);
            break;
        default: 
            enemy->setWeapon(0.6f, 0.1f, 0.1f, 40, 60, 15);
            enemy->setAnimation("greenEnemy.json");
            enemy->setMaxSpeed(300.0f, 800.0f);
            enemy->setAIDistances(150, 50);
            enemy->setRangedMode(false);
            enemy->setMaxHealth(90);
            enemy->setResistances(0.0f, 0.0f, 0.0f);
            break;
    }

    enemy->setSize(size);
    
    return enemy;
}
