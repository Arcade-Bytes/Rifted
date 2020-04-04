#ifndef RANGEDWEAPON_H
#define RANGEDWEAPON_H

#include "Weapon.h"
#include "Projectile.h"

class RangedWeapon : public Weapon
{
private:
    /* data */
    std::vector<Projectile*>* projectileArray;
    bool& b_ownerFacingRight;
    bool b_isPlayer;
public:
    RangedWeapon(const float& cooldown, const float& timeToAttack, const float& damage, bool isPlayer, bool& b_ownerFacing);
    ~RangedWeapon();

    void linkWorldProjectiles(std::vector<Projectile*>& proyectileArray);

    // metodo virtual redefinido
    void attack();
};

#endif
