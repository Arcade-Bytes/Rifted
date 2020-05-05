#include "RangedWeapon.h"

RangedWeapon::RangedWeapon(
    const float& cooldown, const float& timeToAttack,
    const float& damage, float shootHeightOffset,
    bool isPlayer, bool& ownerFacing,
    sf::Vector2f knockback) 
: Weapon(cooldown, timeToAttack, 0, 0, 0, damage, isPlayer, knockback, RANGED_ATTACK),
    b_ownerFacingRight(ownerFacing), b_isPlayer(isPlayer)
{
    this->projectileArray = NULL;
    this->vf_shootHeightOffset = {0,shootHeightOffset};
}

RangedWeapon::~RangedWeapon()
{

}

void RangedWeapon::linkWorldProjectiles(std::vector<Projectile*>& proyectileArrayRef)
{
    this->projectileArray = &proyectileArrayRef;
}

void RangedWeapon::scale(sf::Vector2f scaleRatio)
{
    Weapon::scale(scaleRatio);
    this->vf_shootHeightOffset.x *= scaleRatio.x;
    this->vf_shootHeightOffset.y *= scaleRatio.y;
}

void RangedWeapon::attack()
{
    if(this->projectileArray)
    {
        this->projectileArray->push_back(
            new Projectile(this->getPosition() + this->vf_shootHeightOffset,
            {this->b_ownerFacingRight ? 1.f : -1.f, 0},
            1000.f,
            this->hitbox->getDamage(),
            this->b_isPlayer,
            this->hitbox->getKnockback()
        ));
    }
}
