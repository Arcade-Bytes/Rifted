#include "Entity.h"

Entity::Entity(const float& maxHealth)
{
    this->shape.setSize(sf::Vector2f(50,50)); // Default / Base size

    this->f_maxHealth = maxHealth;
    this->f_currentHealth = this->f_maxHealth;
    this->b_isDead = false;

    this->i_wallCollision = this->i_nearPlatformEnd = 0;
    this->b_isGrounded = false;
    this->b_facingRight = true;

    this->b_isInvulnerable = false;
    this->f_invulnerabilityTime = 0.0f;
    this->f_invulnerableBlinkDelta = 0.0f;
    this->f_invulnerableBlinkTime = 0.1f;
    this->b_isBlinking = false;

    this->f_jumpForce = 1200.0f;
    
    this->movement = new MovementComponent(&this->vf_position, 400.0f, 450.33f, sf::Vector2f(400.0f, 800.0f));
    this->hitbox = NULL;
    this->animation = NULL;

    this->spriteTexture = new sf::Texture();
    this->spriteTexture->loadFromFile("resources/snobees.png");
    this->shape.setTexture(spriteTexture);
    this->initSize(sf::Vector2f(50,50));
    this->setPosition(350,350);
}

Entity::~Entity()
{
    delete this->movement;
    if(this->hitbox) delete this->hitbox;
    if(this->animation) delete this->animation;
    delete this->spriteTexture;
}

void Entity::initSize(sf::Vector2f size)
{
    this->shape.setSize(size);
    this->shape.setOrigin(this->shape.getSize().x/2,this->shape.getSize().y/2);
    if(this->hitbox) this->hitbox->setSize(size.x, size.y);
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
    if(this->hitbox) this->hitbox->setPosition(this->vf_position.x, this->vf_position.y);
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

void Entity::getHurt(float& damage)
{
    f_currentHealth -= damage;
    if(f_currentHealth <= 0.0f)
    {
        this->die();
    }
}

void Entity::getHealed(float& healing)
{
    f_currentHealth += healing;
    if(f_currentHealth >= f_maxHealth)
    {
        f_currentHealth = f_maxHealth;
    }
}

void Entity::die()
{
    this->b_isDead = true;
}

bool Entity::isDead()
{
    return this->b_isDead;
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

void Entity::knockback(const float& xforce, const float& yforce)
{
    this->movement->jump(xforce, yforce);
}

void Entity::checkCollisions()
{
    // Status info reset
    this->i_wallCollision = this->i_nearPlatformEnd = 0;

    this->b_isGrounded = false;
    std::vector<Hitbox*>* hitboxes = Hitbox::getAllHitboxes();
    for(auto hitbox : *hitboxes)
    {
        // Check platform collisions
        if(this->checkObstacle(hitbox))
        {
            // Check how much they collided and push the entity out of the platform
            sf::Vector2f intersection = this->hitbox->checkCollision(hitbox);
            if(intersection.x != 0.0f || intersection.y != 0.0f)
            {
                float difference = abs(intersection.x - intersection.y);
                if(difference <= 10)
                {
                    this->movement->undoMove(1,1);
                    this->movement->stop();
                    this->vf_position.x += intersection.x;
                    this->vf_position.y += intersection.y;
                    printf("Corner (%f)!\n", difference);
                }
                if(abs(intersection.y) < abs(intersection.x))
                {
                    // Stepping on a platform
                    if(intersection.y < 0) {
                        this->movement->undoMove(0,1);
                        this->movement->stopY();
                        this->b_isGrounded = true;
                        // Stepping near the platform corner
                        if(abs(intersection.x) <= this->getSize().x)
                        {
                            if(intersection.x < 0) i_nearPlatformEnd = -1;
                            else                   i_nearPlatformEnd = 1;
                        } 
                    }
                    // Colliding with the roof
                    else
                    {
                        this->vf_position.y += intersection.y;
                        this->movement->stopY();
                    }
                }
                else
                {
                    this->movement->undoMove(1,0);
                    this->movement->stopX();

                    if(intersection.x > 0) i_wallCollision = -1;
                    else                   i_wallCollision = 1;
                }
                this->setPosition(this->vf_position);
            }
        }

        // Check damage areas
        if(this->checkInteraction(hitbox))
        {
            sf::Vector2f intersection = this->hitbox->checkCollision(hitbox);
            if(!this->b_isInvulnerable && (intersection.x != 0.0f || intersection.y != 0.0f))
            {
                float damage = hitbox->getDamage();
                this->getHurt(damage);

                sf::Vector2f diff = this->hitbox->getPosition() - hitbox->getPosition();
                this->knockback(
                    500.0f * (diff.x < 0 ? 1 : -1),
                    200.0f
                );

                this->b_isInvulnerable = true;
                this->f_invulnerabilityTime = 1.0f;

                if(hitbox->getType() == LETHAL)
                {
                    printf("Oh me caí we\n");
                }
            }
        }
    }
}

void Entity::updateAnimation()
{
    if(this->animation)
    {
        this->s_currentAnimation = "idle";
        if(abs(this->movement->getSpeed().x) > 0)
        {
            if(this->b_facingRight) this->s_currentAnimation = "walking_right";
            else                    this->s_currentAnimation = "walking_left";
        }
        else
        {
            if(this->b_facingRight) this->s_currentAnimation = "idle";
            else                    this->s_currentAnimation = "idle";
        }

        this->animation->playAnimation(this->s_currentAnimation);
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

void Entity::updateInvulnerability()
{
    if(this->b_isInvulnerable)
    {
        float delta = Engine::getInstance()->getDelta();
        this->f_invulnerabilityTime -= delta;

        // Blinking
        this->f_invulnerableBlinkDelta += delta;
        if(this->f_invulnerableBlinkDelta >= this->f_invulnerableBlinkTime)
        {
            this->f_invulnerableBlinkDelta -= this->f_invulnerableBlinkTime;
            this->b_isBlinking = !this->b_isBlinking;
            this->shape.setFillColor(this->b_isBlinking ? sf::Color::Transparent : sf::Color::White);
        }

        if(this->f_invulnerabilityTime <= 0.0f)
        {
            this->b_isInvulnerable = false;
            this->shape.setFillColor(sf::Color::White);
        }
    }
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
    this->updateAnimation();
    this->updateInvulnerability();
}

void Entity::render()
{
    Engine::getInstance()->renderDrawable(&shape);
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
}
