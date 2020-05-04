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

    this->b_mutexAttack = false;

    this->b_isInvulnerable = false;
    this->f_invulnerabilityTime = 0.0f;
    this->f_invulnerableBlinkDelta = 0.0f;
    this->f_invulnerableBlinkTime = 0.1f;
    this->b_isBlinking = false;

    this->f_jumpForce = 1200.0f;
    
    this->movement = new MovementComponent(&this->vf_position, 400.0f, 650.33f, sf::Vector2f(400.0f, 800.0f));
    this->hitbox = NULL;
    this->collisionArea = new Hitbox(NO_COLLISION, 0,0, 0,0);;
    this->animation = NULL;
    this->collisionsCalculated = 5;

    this->initSize(sf::Vector2f(50,50));
    this->setPosition(350,350);
    this->vf_previousPosition = this->vf_nextPosition = this->vf_position;
}

Entity::~Entity()
{
    delete this->movement;
    if(this->hitbox) delete this->hitbox;
    if(this->collisionArea) delete this->collisionArea;
    if(this->animation) delete this->animation;
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
    if(this->hitbox) this->hitbox->setPosition(this->vf_position);
}

void Entity::initPosition(sf::Vector2f pos)
{
    this->setPosition(pos);
    this->vf_previousPosition = this->vf_nextPosition = this->vf_position;
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

void Entity::setResistances(float lightRes, float heavyRes, float rangedRes)
{
    this->resistances[LIGHT_ATTACK] = lightRes;
    this->resistances[HEAVY_ATTACK] = heavyRes;
    this->resistances[RANGED_ATTACK] = rangedRes;
}

float Entity::getResistance(DamageType type)
{
    if(resistances.find(type) != resistances.end())
    {
        return resistances[type];
    }
    else return 0.0f;
}

float Entity::getHurt(float& damage)
{
    float healthBefore = f_currentHealth;

    f_currentHealth -= damage;
    if(f_currentHealth <= 0.0f)
    {
        this->die();
    }
    return healthBefore - this->f_currentHealth;
}

float Entity::getHealed(float& healing)
{
    float healthBefore = f_currentHealth;

    f_currentHealth += healing;
    if(f_currentHealth >= f_maxHealth)
    {
        f_currentHealth = f_maxHealth;
    }
    return this->f_currentHealth - healthBefore;
}

void Entity::die()
{
    this->b_isDead = true;
}

void Entity::trulyDie()
{
    this->die();
}

void Entity::revive()
{
    this->b_isDead = false;
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

void Entity::stopSpeed()
{
    this->movement->stop();
}

void Entity::checkCollisions()
{
    // Status info reset
    this->i_wallCollision = this->i_nearPlatformEnd = 0;
    this->b_isGrounded = false;

    this->calculateCollisionArea();

    std::vector<Hitbox*>* hitboxes = Hitbox::getAllHitboxes();
    for(unsigned int i=0; i<hitboxes->size(); i++)
    {
        Hitbox* hitbox = (*hitboxes)[i];

        if(this->collisionArea->checkBooleanCollision(hitbox))
        {
            // Check platform collisions
            bool collided = this->checkObstacleCollision(hitbox);
            if(collided)
            {
                // Update collision area
                this->setPosition(this->vf_position);
                this->calculateCollisionArea();
            }

            // Check damage areas
            this->checkInteractionCollision(hitbox);
        }
    }
}

bool Entity::checkObstacleCollision(Hitbox* hitbox)
{
    float xThreshold = this->getSize().x/32;
    float yThreshold = this->getSize().y/32;
    bool result = false;

    if(this->checkObstacle(hitbox))
    {
        // Copy the destination
        sf::Vector2f currentPos = this->vf_position;

        // Check collision with interpolation
        for(int i=1; i<=collisionsCalculated && !result; i++)
        {
            this->setPosition(
                this->vf_nextPosition + ((currentPos - this->vf_nextPosition) * (float)i*(1.0f/collisionsCalculated))
            );
            // Check how much they collided and push the entity out of the platform
            sf::Vector2f intersection = this->hitbox->checkCollision(hitbox);
            if(intersection.x != 0.0f || intersection.y != 0.0f)
            {
                result = true;
                if(abs(intersection.x) <= xThreshold && abs(intersection.y) <= yThreshold)
                {
                    this->movement->stop();
                    this->vf_position.x += intersection.x;
                    this->vf_position.y += intersection.y;
                }
                if(abs(intersection.y) < abs(intersection.x))
                {
                    if(abs(intersection.y) <= yThreshold)
                        this->vf_position.y = this->vf_nextPosition.y;
                    else
                        this->vf_position.y += intersection.y;
                    this->movement->stopY();
                    this->vf_position.x = currentPos.x;

                    // Stepping on a platform
                    if(intersection.y < 0) {
                        this->b_isGrounded = true;
                        // Stepping near the platform corner
                        if(abs(intersection.x) <= this->getSize().x)
                        {
                            if(intersection.x < 0) i_nearPlatformEnd = -1;
                            else                   i_nearPlatformEnd = 1;
                        } 
                    }
                }
                else
                {
                    if(abs(intersection.x) <= xThreshold)
                        this->vf_position.x = this->vf_nextPosition.x;
                    else
                        this->vf_position.x += intersection.x;
                    this->movement->stopX();
                    this->vf_position.y = currentPos.y;

                    if(intersection.x > 0) i_wallCollision = -1;
                    else                   i_wallCollision = 1;
                }
            }
        }
        if(!result) this->setPosition(currentPos);
        else        this->setPosition(this->vf_position);
    }
    return result;
}

bool Entity::checkInteractionCollision(Hitbox* hitbox)
{
    if(this->checkInteraction(hitbox))
    {
        sf::Vector2f intersection = this->hitbox->checkCollision(hitbox);
        if(intersection.x != 0.0f || intersection.y != 0.0f)
        {
            if(!this->b_isInvulnerable)
            {
                float damage = hitbox->getDamage();
                damage *= 1 - this->getResistance(hitbox->getDamageType());
                float finalDamage = this->getHurt(damage);

                sf::Vector2f diff = this->hitbox->getPosition() - hitbox->getPosition();
                sf::Vector2f knockback = hitbox->getKnockback();
                this->movement->stopY();
                this->knockback(
                    knockback.x * (diff.x < 0 ? 1 : -1),
                    knockback.y
                );

                if(finalDamage > 0.0f)
                {
                    this->b_isInvulnerable = true;
                    this->f_invulnerabilityTime = 1.0f;
                }
            }

            if(hitbox->getType() == LETHAL)
            {
                this->trulyDie();
            }
        }
        return true;
    }
    return false;
}

void Entity::calculateCollisionArea()
{
    sf::Vector2f size;
    sf::Vector2f pos;

    // Calculate size
    size = this->vf_position - this->vf_nextPosition;
    size.x = abs(size.x) + this->getSize().x;
    size.y = abs(size.y) + this->getSize().y;

    // Calculate position
    if(this->vf_position.x < this->vf_nextPosition.x)   pos.x = this->vf_position.x;
    else                                                    pos.x = this->vf_nextPosition.x;
    if(this->vf_position.y < this->vf_nextPosition.y)   pos.y = this->vf_position.y;
    else                                                    pos.y = this->vf_nextPosition.y;

    // Adjust position
    pos.x -= this->getSize().x/2.0f;
    pos.y -= this->getSize().y/2.0f;
    pos.x += size.x / 2.0f;
    pos.y += size.y / 2.0f;

    // Increase the size a bit
    size.x += this->getSize().x / 4.0f;
    size.y += this->getSize().y / 4.0f;

    this->collisionArea->setSize(size.x, size.y);
    this->collisionArea->setPosition(pos.x, pos.y);
}

void Entity::updateAnimation()
{
    if(this->animation)
    {
        this->s_currentAnimation = "idle_right";
        if(this->b_mutexAttack)
        {
            if(this->b_facingRight) this->s_currentAnimation = "pushing_right";
            else                    this->s_currentAnimation = "pushing_left";
        }
        else if(abs(this->movement->getSpeed().x) > 0)
        {
            if(this->b_facingRight) this->s_currentAnimation = "walking_right";
            else                    this->s_currentAnimation = "walking_left";
        }
        else
        {
            if(this->b_facingRight) this->s_currentAnimation = "idle_right";
            else                    this->s_currentAnimation = "idle_left";
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
    this->updateInterpolationPositions();
}

void Entity::render(float frameProgress)
{
    this->shape.setPosition(this->getInterpolatedPosition(frameProgress));
    Engine::getInstance()->renderDrawable(&shape);
}

sf::Vector2f Entity::getInterpolatedPosition(float frameProgress)
{
    return this->vf_previousPosition + (this->vf_nextPosition - this->vf_previousPosition) * frameProgress;
}

void Entity::updateInterpolationPositions()
{
    this->vf_previousPosition = this->vf_nextPosition;
    this->vf_nextPosition = this->vf_position;
}

std::string Entity::getMaxHealth()
{
    return std::to_string(this->f_maxHealth).c_str();
}

std::string Entity::getHealth()
{
    return std::to_string(this->f_currentHealth).c_str();
}

float Entity::getFloatHealth() 
{
    return this->f_currentHealth;
}
float Entity::getFloatMaxHealth() 
{
    return this->f_maxHealth;
}

void Entity::setHealth(float f_health)
{
    this->f_currentHealth = f_health;
}

void Entity::setMaxHealth(float maxHealth)
{
    this->f_maxHealth = maxHealth;
    this->f_currentHealth = this->f_maxHealth;
}
