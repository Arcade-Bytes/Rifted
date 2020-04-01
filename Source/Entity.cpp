#include "Entity.h"

Entity::Entity()
{
    this->shape.setSize(sf::Vector2f(50,50)); // Default / Base size

    this->b_isGrounded = false;
    this->b_facingRight = true;
    this->f_jumpForce = 500.0f;
    this->movement = new MovementComponent(&this->vf_position, 400.0f, 370.33f, sf::Vector2f(400.0f, 900.0f));
    this->hitbox = new Hitbox(PLAYER, 0,0, 0,0);

    this->shape.setFillColor(sf::Color::White);
    this->initSize(sf::Vector2f(50,50));
    this->setPosition(350,350);
}

Entity::~Entity()
{
    
}

void Entity::initSize(sf::Vector2f size)
{
    this->shape.setSize(size);
    this->shape.setOrigin(this->shape.getSize().x/2,this->shape.getSize().y/2);
    this->hitbox->setSize(size.x, size.y);
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

sf::Vector2f Entity::getSize()
{
    return this->hitbox->getSize();
}

void Entity::setSize(sf::Vector2f size)
{
    sf::Vector2f ratio = {
        size.x / this->shape.getSize().x,
        size.y / this->shape.getSize().y
    };
    this->initSize(size);
    this->movement->resize(ratio);
    this->f_jumpForce *= ratio.y;
    this->resizeItems(ratio);
}

Hitbox* Entity::getHitbox()
{
    return this->hitbox;
}

void Entity::move(const float& xdir)
{
    this->movement->move(xdir);
    this->b_facingRight = (xdir >= 0);
}

void Entity::jump(const float& xnormalized, const float& ynormalized)
{
    if(this->b_isGrounded)
    {
        this->movement->stopY();
        this->movement->jump(f_jumpForce*xnormalized, f_jumpForce*ynormalized);
        this->b_isGrounded = false;
    }
}

void Entity::checkCollisions()
{
    // Status info
    bool fallingLeft = false, fallingRight = false, collidingLeft = false, collidingRight = false;

    this->b_isGrounded = false;
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
                float difference = abs(intersection.x - intersection.y);
                //if(difference <= 50) printf("Oh no there is %f difference (%f - %f)\n", difference, intersection.x, intersection.y);
                if(difference <= 10)
                {
                    /*this->movement->undoMove(1,1);
                    this->movement->stop();
                    this->vf_position.x += intersection.x;
                    this->vf_position.y += intersection.y;*/
                    printf("Corner!\n");
                }
                if(abs(intersection.y) < abs(intersection.x))
                {
                    this->movement->undoMove(0,1);
                    this->movement->stopY();

                    // Stepping on a platform
                    if(intersection.y < 0) {
                        this->b_isGrounded = true;
                        // Stepping near the platform corner
                        if(abs(intersection.x) <= this->getSize().x)
                        {
                            if(intersection.x < 0) fallingLeft = true;
                            else                   fallingRight = true;
                        } 
                    }
                    // Colliding with the roof
                    else
                    {
                        this->vf_position.y += intersection.y*0.1;
                    }
                }
                else
                {
                    this->movement->undoMove(1,0);
                    this->movement->stopX();

                    if(intersection.x > 0) collidingLeft = true;
                    else                   collidingRight = true;
                }
            }
        }
        //printf("Status is %d, %d, %d, %d\n", fallingLeft, fallingRight, collidingLeft, collidingRight);
    }
}

void Entity::updateMovement()
{
    this->movement->update();
    this->setPosition(this->vf_position);

    // Platform Collision detection
    this->checkCollisions();

    this->setPosition(this->vf_position);
}

bool Entity::updateWeapon(Weapon* weapon)
{
    weapon->setPosition(this->vf_position.x, this->vf_position.y, b_facingRight);
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
}
