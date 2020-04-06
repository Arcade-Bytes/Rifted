#include "Shield.h"

Shield::Shield(const float& activationTime, const float& recoveryTime, const float& parryWindow, const float& parryHeal)
{
    this->f_activationTime = activationTime;
    this->f_recoveryTime = recoveryTime;
    this->f_parryWindow = parryWindow;
    this->f_parryHeal = parryHeal;

    this->shape.setTexture(ResourceManager::getInstance()->loadTexture("resources/shield.png"));
    this->shape.setSize(sf::Vector2f(40,40));
    this->shape.setOrigin(this->shape.getSize().x/2,this->shape.getSize().y/2);
    this->i_upgradeLvl = -1;
}

Shield::~Shield()
{

}

void Shield::RiseShield()
{
    float delta = Engine::getInstance()->getDelta();

    if(!b_isActive)
    {
        // Add delta time to drawn time
        f_drawn += delta;

        if(f_drawn >= f_activationTime)
        {
            b_isActive = true;
            f_drawn = f_recoveryTime;
        }
    }
}

void Shield::RestShield()
{
    float delta = Engine::getInstance()->getDelta();

    f_drawn -= delta;
    if(f_drawn <= 0.0f)
    {
        b_isActive = false;
        f_drawn = 0.0f;
    }
}

float Shield::DamageBlock()
{
    float f_damageMultiplier = 1.0f;

    if(b_isActive)
    {
        f_damageMultiplier = 0.0f;
    }
    else
    {
        Parry(f_damageMultiplier);
    }

    return f_damageMultiplier;
}

void Shield::Parry(float& f_damageMultiplier)
{
    // If f_drawn time is between parryWindow and (activationTime - parryWindow)
    // [---+++++++---] (+++++++ = actual parry window) (---+++++++ = f_parryWindow)
    if(f_drawn <= f_parryWindow && f_drawn >= f_activationTime - f_parryWindow)
    {
        b_isActive = true;
        f_damageMultiplier = -f_parryHeal;
    }
}

void Shield::setPosition(const float& xpos, const float& ypos, bool facingRight)
{
    this->shape.setPosition(
        xpos + 20 * (facingRight ? 1 : -1),
        ypos + 5
    );
}

void Shield::scale(sf::Vector2f scaleRatio)
{
    this->shape.scale(scaleRatio);
}

void Shield::render()
{
    if(b_isActive)
        Engine::getInstance()->renderDrawable(&shape);
}

int Shield::getUpgradeLvl()
{
    return this->i_upgradeLvl;
}

void Shield::setUpgradeLvl(int i_lvl)
{
    this->i_upgradeLvl = i_lvl;
}
