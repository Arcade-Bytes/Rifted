#ifndef WEAPON_H
#define WEAPON_H

#include "Engine.h"

class Weapon {
private:
    sf::RectangleShape shape;

    bool b_isAttacking;
    bool b_alreadyAttacked;

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

    void update();
    void render();
};

#endif
