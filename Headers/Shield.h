#ifndef SHIELD_H
#define SHIELD_H

#include "Engine.h"
#include "Hitbox.h"

class Shield {
private:
    bool b_isActive;
    float f_activationTime;
    float f_recoveryTime;
    float f_parryWindow;
    float f_drawn;
    float f_parryHeal;

    // FALSE FACADE!!!!
    sf::RectangleShape shape;
public:
    Shield(const float& activationTime, const float& recoveryTime, const float& parryWindow, const float& parryHeal);
    ~Shield();

    void RiseShield();
    void RestShield();
    float DamageBlock();
    void Parry(float& damageMultiplier); 

    void setPosition(const float& xpos, const float& ypos, bool facingRight);

    void scale(sf::Vector2f scaleRatio);

    void update();
    void render();
};

#endif
