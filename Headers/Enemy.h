#ifndef ENEMY_H
#define ENEMY_H

#include "Engine.h"
#include "Entity.h"
#include "Player.h"

class Enemy : public Entity {
private:
    enum EnemyState {EnemyPatrolling = 0, EnemyChasing, EnemyAttacking};

    bool b_mutexAttack;

    float f_attackDistance;
    float f_aggroDistance;
    bool b_isRanged;
    EnemyState state;
    Player* player;
    Weapon* weapon;

    // AI Properties
    bool b_jumpsOffPlatforms; // true if this AI ignores end of platform and keeps walking

    // AI data
    bool b_patrolLeft;
public:
    Enemy(Player* playerRef);
    ~Enemy();

    // Factory setters
    void setWeapon(const float& cooldown, const float& timeToAttack, const float& window, const float& xsize, const float& ysize, float damage);
    void setRangedWeapon(const float& cooldown, const float& timeToAttack, float damage);
    void setAnimation(std::string animationFile);
    void setMaxSpeed(const float&maxX, const float&maxY);
    void setAIDistances(float aggro, float attack);
    void setRangedMode(bool ranged);

    void attack();

    void updateAI();
    void updateAIState(const float& distance, const float& yDiff);

    void die();
    void trulyDie();

    // Virtual override
    void linkWorldProjectiles(std::vector<Projectile*>& proyectileArray);
    bool checkObstacle(Hitbox* hitbox);
    bool checkInteraction(Hitbox* hitbox);
    void resizeItems(sf::Vector2f scaleRatio);

    void update();
    void render();
};

#endif
