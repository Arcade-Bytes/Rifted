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

    void attack();

    void updateAI();
    void updateAIState(const float& distance);

    bool isOnPlatform();
    bool isOnPlatform(const float& posx, const float& posy);
    bool nextMoveLeavesPlatform(const float& x, const float& y);

    // Virtual override
    void resizeItems(sf::Vector2f scaleRatio);

    void update();
    void render();
};

#endif
