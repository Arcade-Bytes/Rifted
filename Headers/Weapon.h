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

    float f_attackCooldown; // Total time needed to finish the attack
    float f_attackTime;     // Moment when the attack hits and its calculated
    float f_attackDelta;    // Time that passed since the attack started
public:
    Weapon(const float& cooldown, const float& timeToAttack, const float& xsize, const float& ysize);
    ~Weapon();

    bool isAttacking();

    void setPosition(const float& xpos, const float& ypos, bool facingRight);
    void startAttack();
    void updateAttack();
    void attack();

    void scale(sf::Vector2f scaleRatio);

    void update();
    void render();
};

#endif
