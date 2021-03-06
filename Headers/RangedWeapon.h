#ifndef RANGEDWEAPON_H
#define RANGEDWEAPON_H

#include "Weapon.h"
#include "Projectile.h"

class RangedWeapon : public Weapon
{
private:
    /* data */
    std::vector<Projectile*>* projectileArray;
    sf::Vector2f vf_shootHeightOffset;
    bool& b_ownerFacingRight;
    bool b_isPlayer;
public:
    RangedWeapon(const float& cooldown, const float& timeToAttack, const float& damage, float shootHeightOffset,
        bool isPlayer, bool& b_ownerFacing, sf::Vector2f knockback = {0.0f,0.0f});
    ~RangedWeapon();

    void linkWorldProjectiles(std::vector<Projectile*>& proyectileArray);
    void scale(sf::Vector2f scaleRatio);

    // metodo virtual redefinido
    void attack();
};

#endif
