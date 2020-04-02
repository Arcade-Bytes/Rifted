#include "Weapon.h"

Weapon::Weapon(const float& cooldown, const float& timeToAttack, const float& window, const float& xsize, const float& ysize, float damage, bool isPlayer)
    : f_attackCooldown(cooldown), f_attackTime(timeToAttack), f_attackWindow(window)
{
    this->vf_size = {xsize, ysize};
    this->b_isAttacking = false;
    this->f_reach = 30.0f;

    this->hitbox = new Hitbox(isPlayer ? PLAYER_ATTACK : ENEMY_ATTACK, 0,0, 0,0, damage);
    this->hitbox->setColor(sf::Color(55,55,55,200));
    b_isAttacking = false;
    
    this->i_upgradeLevel = -1;
}

Weapon::~Weapon()
{
    delete this->hitbox;
}

bool Weapon::isAttacking()
{
    return b_isAttacking;
}

void Weapon::setPosition(const float& xpos, const float& ypos, bool facingRight)
{
    this->hitbox->setPosition(
        xpos + this->f_reach * (facingRight ? 1 : -1),
        ypos
    );
}

void Weapon::startAttack()
{
    if(!b_isAttacking) {
        f_attackDelta = 0.0;
        b_alreadyAttacked = false;
        b_isAttacking = true;
    }
}

void Weapon::updateAttack()
{
    // Update the delta time value of the attack
    float delta = Engine::getInstance()->getDelta();
    f_attackDelta += delta;

    // If the attack has reached the point when it attacks...
    if(!b_alreadyAttacked && f_attackDelta >= f_attackTime)
    {
        b_alreadyAttacked = true;
        this->hitbox->setSize(vf_size.x, vf_size.y);
    }

    // If the attack finished
    if(f_attackDelta >= f_attackCooldown)
    {
        b_isAttacking = false;
        this->hitbox->setSize(0.0f, 0.0f);
    }
}

void Weapon::scale(sf::Vector2f scaleRatio)
{
    this->vf_size.x *= scaleRatio.x;
    this->vf_size.y *= scaleRatio.y;
    this->f_reach *= scaleRatio.x;
}

void Weapon::update()
{
    if(b_isAttacking) updateAttack();
}

void Weapon::render()
{
    if(b_isAttacking)
    {
        this->hitbox->render();
    }
}

int Weapon::getUpgradeLvl()
{
    return this->i_upgradeLevel;
}

void Weapon::setUpgradeLvl(int i_lvl)
{
    this->i_upgradeLevel = i_lvl;
}
