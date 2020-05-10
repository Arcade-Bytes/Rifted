#ifndef SHIELD_H
#define SHIELD_H

#include "Engine.h"
#include "Hitbox.h"
#include "ResourceManager.h"

class Shield {
private:
    bool b_isActive;
    float f_activationTime;
    float f_recoveryTime;
    float f_parryWindow;
    float f_drawn;
    float f_parryHeal;

    int i_upgradeLvl;

    sf::RectangleShape shape;
public:
    Shield(const float& activationTime, const float& recoveryTime, const float& parryWindow, const float& parryHeal);
    ~Shield();

    /**
     * Shield State and damage control
     **/
    void RiseShield();
    void RestShield();
    float DamageBlock();

    /**
     * Parry mechanic
     **/
    void Parry(float& damageMultiplier); 


    /**
     * Getters and setters
     **/
    bool isUp();
    void setPosition(const float& xpos, const float& ypos, bool facingRight);
    int getUpgradeLvl();
    void setUpgradeLvl(int i_lvl);

    void scale(sf::Vector2f scaleRatio);

    void render();
};

#endif
