#include "RangedWeapon.h"

RangedWeapon::RangedWeapon(
    const float& cooldown, const float& timeToAttack,
    const float& damage, bool isPlayer, bool& ownerFacing) 
: Weapon(cooldown, timeToAttack, 0, 0, 0, damage, isPlayer), b_ownerFacingRight(ownerFacing), b_isPlayer(isPlayer), f_damage(damage)
{
    this->projectileArray = NULL;
}

RangedWeapon::~RangedWeapon()
{

}

void RangedWeapon::linkWorldProjectiles(std::vector<Projectile*>& proyectileArrayRef)
{
    this->projectileArray = &proyectileArrayRef;
}

void RangedWeapon::attack()
{
    if(this->projectileArray)
    {
        this->projectileArray->push_back(
            new Projectile(this->getPosition(),
            {this->b_ownerFacingRight ? 1.f : -1.f, 0},
            1000.f,
            this->f_damage,
            this->b_isPlayer
        ));
    }
}
