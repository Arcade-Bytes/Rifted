#include "Entity.h"

Entity::Entity()
{
    this->movement = new MovementComponent(&this->vf_position, 25.0f, 10.33f, 5.0f);
    this->vf_position = {350,350};
    this->shape.setFillColor(sf::Color::White);
    this->shape.setSize(sf::Vector2f(50,50));
    this->shape.setOrigin(this->shape.getSize().x/2,this->shape.getSize().y/2);
    this->shape.setPosition(this->vf_position);

    this->hitbox = new Hitbox(PLAYER, 50,50, 350,350);
}

Entity::~Entity()
{
    
}

sf::Vector2f Entity::getPosition()
{
    return this->vf_position;
}

void Entity::move(const float& xdir)
{
    this->movement->move(xdir);
}

void Entity::jump(const float& yforce)
{
    this->movement->jump(yforce);
}

void Entity::updateMovement()
{
    this->movement->update();
    this->hitbox->setPosition(this->vf_position.x, this->vf_position.y);

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
                printf("Collision -> (%f, %f)\n", intersection.x, intersection.y);
                this->vf_position += intersection;

                if(abs(intersection.y) > abs(intersection.x))
                {
                    this->movement->stopY();
                }
                else
                {
                    this->movement->stopX();
                }
                this->hitbox->setPosition(this->vf_position.x, this->vf_position.y);
            }
        }
    }

    this->shape.setPosition(this->vf_position);
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
    Engine::getInstance()->RenderShape(&shape);
}
