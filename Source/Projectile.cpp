#include "Projectile.h"

// std::vector<Projectile*> Projectile::Projectiles;

Projectile::Projectile(sf::Vector2f position, sf::Vector2f direction, float speed, float damage, bool isPlayer)
{
    this->vf_position = position;
    this->vf_direction = direction;
    this->f_speed = speed;
    this->b_isDestroyed = false;

    this->hitbox = new Hitbox(isPlayer ? PLAYER_ATTACK : ENEMY_ATTACK, 60, 10, 0, 0, damage);

    this->shape.setPosition(this->vf_position);
    this->shape.setSize({60, 10});
    this->shape.setFillColor(sf::Color(128,90,70));

    // Projectile::Projectiles.push_back(this);
}

Projectile::~Projectile()
{
    delete this->hitbox;
}

void Projectile::move()
{
    sf::Vector2f movement = this->vf_direction * this->f_speed * Engine::getInstance()->getDelta();

    this->shape.move(movement);
    this->vf_position += movement;
}

void Projectile::updateMovement()
{
    this->move();
    this->hitbox->setPosition(this->vf_position.x, this->vf_position.y);
}

void Projectile::checkCollisions()
{
    std::vector<Hitbox*>* hitboxes = Hitbox::getAllHitboxes();
    for(auto hitbox : *hitboxes)
    {
        if(checkInteraction(hitbox->getType()))
        {
            sf::Vector2f intersection = this->hitbox->checkCollision(hitbox);
            if(intersection.x != 0.0f || intersection.y != 0.0f)
            {
                this->b_isDestroyed = true;
                break;
            }
        }
    }
}

bool Projectile::checkInteraction(HitboxType type)
{
    bool result = true;
    if(this->hitbox->getType() == PLAYER_ATTACK)
    {
        switch(type)
        {
            case PLAYER_ATTACK:
            case PLAYER:
            result = false; break;
            default: break;
        }
    }
    else
    {   
        switch(type)
        {
            case ENEMY_ATTACK:
            case ENEMY:
            result = false; break;
            default: break;
        }
    }

    return result;
}

bool Projectile::isDestroyed()
{
    return this->b_isDestroyed;
}

void Projectile::update()
{
    this->updateMovement();
    this->checkCollisions();
}

void Projectile::render()
{
    Engine::getInstance()->renderDrawable(&this->shape);
}