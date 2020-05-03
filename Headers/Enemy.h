#ifndef ENEMY_H
#define ENEMY_H

#include "Engine.h"
#include "Entity.h"
#include "Player.h"

class Enemy : public Entity {
public:
    enum EnemyType {BasicMelee = 1, BasicRanged = 2, BreakableWall=3, Platform=4};
private:
    enum EnemyState {EnemyPatrolling = 0, EnemyChasing, EnemyAttacking};

    int pointType;

    float f_attackDistance;
    float f_aggroDistance;
    bool b_isRanged;
    EnemyState state;
    EnemyType type;
    Player* player;
    Weapon* weapon;

    // AI Properties
    bool b_jumpsOffPlatforms; // true if this AI ignores end of platform and keeps walking
    bool b_doesPatrol;
    bool b_isDying;
    float f_attackCooldown;
    float f_deathTime;

    // AI data
    bool b_patrolLeft;
    float f_attackCooldownCounter;
    float f_deathTimeCounter;
public:
    Enemy(Player* playerRef, int type);
    ~Enemy();

    // Factory setter: Sets the weapon data (melee weapon only, see setRangedWeapon for ranged)
    void setWeapon(const float& cooldown, const float& timeToAttack, const float& window, const float& xsize, const float& ysize,
        float damage, sf::Vector2f knockback = {0.0f,0.0f}, DamageType dmgType = LIGHT_ATTACK);
    // Factory setter: Sets the ranged weapon data (use setRangedMode to true for correct behavior)
    void setRangedWeapon(const float& cooldown, const float& timeToAttack, float damage, sf::Vector2f knockback = {0.0f,0.0f});
    // Factory setter: Sets the cooldown time the enemy must wait between the end of an attack and the start of the next
    void setAttackCooldown(float cooldown);
    // Factory setter: Sets the .json animation file with the animation data
    void setAnimation(std::string animationFile);
    // Factory setter: Sets the max speed. This will be scaled later with setSize
    void setMaxSpeed(const float&maxX, const float&maxY);
    // Factory setter: Sets the AI distances, aggro is the distance needed to see the player, attack is the distance needed to attack the player
    void setAIDistances(float aggro, float attack);
    // Factory setter: Sets the ranged mode behaviour (recommended if weapon is ranged)
    void setRangedMode(bool ranged);
    // Factory setter: Enables patrolling for this enemy. True means that this enemy will move from left to right if idle
    // If false, this enemy will stay where it is when idle
    void setDoPatrol(bool patrol);
    // Factory setter: Sets how much it takes to disappear after death, while the "dead" animation plays
    void setDeathAnimationTime(float deathTime);
    // Factory setter: Sets the enemy type: melee, ranged, wall or platform.
    void setEnemyType(EnemyType type);
    // Factory setter: Change the hitbox type from Enemy to another one
    void overrideHitboxType(HitboxType type);

    void attack();

    void updateAI();
    void updateAIState(const float& distance, const float& yDiff);

    void die();
    void trulyDie();

    int getType();

    // Virtual override
    void linkWorldProjectiles(std::vector<Projectile*>& proyectileArray);
    bool checkObstacle(Hitbox* hitbox);
    bool checkInteraction(Hitbox* hitbox);
    void resizeItems(sf::Vector2f scaleRatio);

    void updateAnimation();
    void update();
    void render(float frameProgress);
};

#endif
