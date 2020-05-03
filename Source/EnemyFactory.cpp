#include "EnemyFactory.h"

EnemyFactory::EnemyFactory()
{

}

EnemyFactory::~EnemyFactory()
{
    
}

Enemy* EnemyFactory::makeEnemy(Player* player, sf::Vector2f size, int type)
{
    Enemy* enemy = new Enemy(player,type);

    // Esqueleto melee (light)
    // Esqueleto arquero (ranged)
    // Esqueleto grande (heavy)
    // Esqueleto arquero grande (ranged)
    // Golem (mina) resiste a todo menos martillo (heavy)
    // Torreta

    // Boss final
    switch(type)
    {
        case 1: // Basic melee
            enemy->setWeapon(0.6f, 0.1f, 0.1f, 40, 60, 15, {1500.0f,500.0f}, LIGHT_ATTACK);
            enemy->setEnemyType( Enemy::EnemyType::BasicMelee );
            enemy->setAnimation("melee_enemy.json");
            enemy->setMaxSpeed(300.0f, 800.0f);
            enemy->setAIDistances(150, 50);
            enemy->setRangedMode(false);
            enemy->setMaxHealth(90);
            enemy->setResistances(0.0f, 0.0f, 0.0f);
            break;
        case 2: // Basic ranged
            enemy->setRangedWeapon(2.0f, 0.1f, 10);
            enemy->setEnemyType( Enemy::EnemyType::BasicRanged );
            enemy->setAnimation("archer.json");
            enemy->setMaxSpeed(500.0f, 800.0f);
            enemy->setAIDistances(650, 600);
            enemy->setRangedMode(true);
            enemy->setMaxHealth(50);
            enemy->setResistances(0.0f, 0.0f, 0.0f);
            break;
        case 3: // Heavy melee
            enemy->setWeapon(0.6f, 0.1f, 0.1f, 40, 60, 30, {1500.0f,0.0f}, HEAVY_ATTACK);
            enemy->setEnemyType( Enemy::EnemyType::BasicMelee );
            enemy->setAnimation("melee_enemy.json");
            enemy->setMaxSpeed(300.0f, 800.0f);
            enemy->setAIDistances(150, 50);
            enemy->setRangedMode(false);
            enemy->setMaxHealth(150);
            enemy->setResistances(0.0f, 0.0f, 0.0f);
            break;
        case 4: // Heavy ranged
            enemy->setRangedWeapon(2.0f, 0.1f, 20);
            enemy->setEnemyType( Enemy::EnemyType::BasicRanged );
            enemy->setAnimation("archer.json");
            enemy->setMaxSpeed(500.0f, 800.0f);
            enemy->setAIDistances(650, 600);
            enemy->setRangedMode(true);
            enemy->setMaxHealth(120);
            enemy->setResistances(0.0f, 0.0f, 0.0f);
            break;
        case 5: // Mine Golem
            enemy->setWeapon(0.6f, 0.1f, 0.1f, 40, 60, 40, {2000.0f,700.0f}, HEAVY_ATTACK);
            enemy->setEnemyType( Enemy::EnemyType::BasicMelee );
            enemy->setAnimation("melee_enemy.json");
            enemy->setMaxSpeed(200.0f, 800.0f);
            enemy->setAIDistances(150, 50);
            enemy->setRangedMode(false);
            enemy->setMaxHealth(200);
            enemy->setResistances(0.5f, 0.0f, 0.9f);
            break;
        case 6: // Automatic Turret
            enemy->setRangedWeapon(2.0f, 0.1f, 20);
            enemy->setEnemyType( Enemy::EnemyType::BasicRanged );
            enemy->setAnimation("archer.json");
            enemy->setMaxSpeed(0.0f, 800.0f);
            enemy->setAIDistances(650, 600);
            enemy->setRangedMode(true);
            enemy->setMaxHealth(120);
            enemy->setResistances(1.0f, 1.0f, 1.0f);
            enemy->setDoPatrol(false);
            break;
        case 10: // Breakable wall
            enemy->setWeapon(0.1f, 0.0f, 0.1f, 0, 0, 0);
            enemy->setAnimation("breakableWall.json");
            enemy->setMaxSpeed(0.0f, 800.0f);
            enemy->setAIDistances(0, 0);
            enemy->setRangedMode(false);
            enemy->setMaxHealth(1.0f);
            enemy->setResistances(1.0f, 0.0f, 1.0f);
            enemy->setDoPatrol(false);
            enemy->overrideHitboxType(PLATFORM);
            break;
        case 11: // Platform enemy
            enemy->setWeapon(0.1f, 0.0f, 0.1f, 0, 0, 0);
            enemy->setAnimation("platform.json");
            enemy->setMaxSpeed(500.0f, 800.0f);
            enemy->setAIDistances(50*1.1, 0);
            enemy->setRangedMode(false);
            enemy->setMaxHealth(1000);
            enemy->setResistances(1.0f, 1.0f, 1.0f);
            enemy->setDoPatrol(false);
            enemy->overrideHitboxType(PLATFORM);
            break;
        default: 
            enemy->setWeapon(0.6f, 0.1f, 0.1f, 40, 60, 15, {1500.0f,500.0f}, LIGHT_ATTACK);
            enemy->setEnemyType( Enemy::EnemyType::BasicMelee );
            enemy->setAnimation("melee_enemy.json");
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
