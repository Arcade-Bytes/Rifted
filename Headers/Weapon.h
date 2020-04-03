#ifndef WEAPON_H
#define WEAPON_H

#include "Engine.h"
#include "Hitbox.h"

class Weapon {
private:
    Hitbox* hitbox;

    bool b_isAttacking;
    bool b_alreadyAttacked;

    float f_reach;
    sf::Vector2f vf_size;

    float f_attackCooldown; // Total time needed to finish the attack
    float f_attackTime;     // Moment when the attack starts and its calculated
    float f_attackWindow;
    float f_attackDelta;    // Time that passed since the attack started

    int i_upgradeLevel;     // The upgrade level of the weapon, -1 means it's not unlocked yet

public:
    Weapon(const float& cooldown, const float& timeToAttack, const float& window, const float& xsize, const float& ysize, float damage, bool isPlayer);
    virtual ~Weapon();

    bool isAttacking();

    void setPosition(const float& xpos, const float& ypos, bool facingRight);
    sf::Vector2f getPosition();

    virtual void attack();
    void startAttack();
    void updateAttack();

    void scale(sf::Vector2f scaleRatio);

    int getUpgradeLvl();
    void setUpgradeLvl(int i_lvl);

    void update();
    void render();
};

#endif
