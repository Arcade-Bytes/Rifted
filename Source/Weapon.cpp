#include "Weapon.h"

Weapon::Weapon(const float& cooldown, const float& timeToAttack, const float& xsize, const float& ysize)
    : f_attackCooldown(cooldown), f_attackTime(timeToAttack)
{
    this->b_isAttacking = false;
    this->f_reach = 30.0f;

    this->hitbox = new Hitbox(WEAPON, xsize, ysize, 0,0);
    this->hitbox->setColor(sf::Color(55,55,55,200));
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
        attack();
    }

    // If the attack finished
    if(f_attackDelta >= f_attackCooldown)
        b_isAttacking = false;
}

void Weapon::attack()
{
    std::cout<<"Attack!\n";
}

void Weapon::scale(sf::Vector2f scaleRatio)
{
    this->hitbox->scale(scaleRatio);
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
