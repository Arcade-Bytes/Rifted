#include "Entity.h"

Entity::Entity()
{
    this->movement = new MovementComponent(&this->vf_position, 25.0f, 10.33f, 5.0f);
    this->shape.setFillColor(sf::Color::White);

    this->hitbox = new Hitbox(PLAYER, 0,0, 0,0);
    this->setSize(sf::Vector2f(50,50));
    this->setPosition(350,350);
<<<<<<< HEAD

=======
>>>>>>> d0f482223e1cd0fa998853caf699f2ef19fe23ae
}

Entity::~Entity()
{
    
}

sf::Vector2f Entity::getPosition()
{
    return this->vf_position;
}

void Entity::setPosition(const float&x, const float& y)
{
    this->setPosition(sf::Vector2f(x,y));
}

void Entity::setPosition(sf::Vector2f pos)
{
    this->vf_position = pos;
    this->shape.setPosition(this->vf_position);
    this->hitbox->setPosition(this->vf_position.x, this->vf_position.y);
}

void Entity::setSize(sf::Vector2f size)
{
    this->shape.setSize(size);
    this->shape.setOrigin(this->shape.getSize().x/2,this->shape.getSize().y/2);
    this->hitbox->setSize(size.x, size.y);
}

Hitbox* Entity::getHitbox()
{
    return this->hitbox;
}

void Entity::move(const float& xdir)
{
    this->movement->move(xdir);
}

void Entity::jump(const float& yforce)
{
    if(this->b_isGrounded)
    {
        this->movement->jump(yforce);
        this->b_isGrounded = false;
    }
}

void Entity::updateMovement()
{
    this->movement->update();
    this->setPosition(this->vf_position);

    // Platform Collision detection
    std::vector<Hitbox*>* hitboxes = Hitbox::getAllHitboxes();
    for(auto platform : *hitboxes)
    {
        // Check platform collisions
        if(platform->getType() == PLATFORM)
        {
            // Check how much they collided and push the entity out of the platform
            sf::Vector2f intersection = this->hitbox->checkCollision(platform);
            if(intersection.x != 0.0f || intersection.y != 0.0f)
            {
                if(abs(intersection.y) > abs(intersection.x))
                {
                    this->movement->undoMove(0,1);
                    this->movement->stopY();

                    if(intersection.y < 0) this->b_isGrounded = true;
                }
                else
                {
                    this->movement->undoMove(1,0);
                    this->movement->stopX();
                }
            }
        }
    }

    this->setPosition(this->vf_position);
}

bool Entity::updateWeapon(Weapon* weapon)
{
    weapon->setPosition(this->vf_position.x, this->vf_position.y, true);
    weapon->update();
    return weapon->isAttacking();
}

void Entity::update()
{
    this->updateMovement();
}

void Entity::render()
{
    Engine::getInstance()->renderDrawable(&shape);
<<<<<<< HEAD
}

std::string Entity::getMaxHealth()
{
    return std::to_string(this->f_maxHealth).c_str();
}

std::string Entity::getHealth()
{
    return std::to_string(this->f_currentHealth).c_str();
}

void Entity::setHealth(float f_health)
{
    this->f_currentHealth = f_health;
}

void Entity::setMaxHealth(float f_maxHealth)
{
    this->f_maxHealth = f_maxHealth;
=======
>>>>>>> d0f482223e1cd0fa998853caf699f2ef19fe23ae
}
