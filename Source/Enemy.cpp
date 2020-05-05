#include "Enemy.h"

Enemy::Enemy(Player* playerRef)
    : Entity(100.0f)
{
    this->hitbox = new Hitbox(ENEMY, this->shape.getSize().x,this->shape.getSize().y, this->vf_position.x,this->vf_position.y);

    // AI Properties and helpers
    this->player = playerRef;
    this->b_patrolLeft = false;
    this->b_doesPatrol = true;

    this->f_aggroDistance = 150;
    this->f_attackDistance = 50;
    this->weapon = NULL;
    this->animation = NULL;
}

Enemy::~Enemy()
{
    delete this->weapon;
}

// Factory setters
void Enemy::setWeapon(const float& cooldown, const float& timeToAttack,
    const float& window, const float& xsize, const float& ysize,
    float damage, sf::Vector2f knockback, DamageType dmgType)
{
    if(this->weapon) delete this->weapon;
    this->weapon = new Weapon(cooldown, timeToAttack, window, xsize, ysize, damage, false, knockback, dmgType);
}

void Enemy::setRangedWeapon(const float& cooldown, const float& timeToAttack, float damage, sf::Vector2f knockback)
{
    if(this->weapon) delete this->weapon;
    this->weapon = new RangedWeapon(cooldown, timeToAttack, damage, false, this->b_facingRight, knockback);
}

void Enemy::setAnimation(std::string animationFile)
{
    if(this->animation) delete this->animation;
    this->animation = new AnimationComponent(this->shape);
    this->animation->loadAnimationsFromJSON("animations/"+animationFile);
}

void Enemy::setMaxSpeed(const float&maxX, const float&maxY)
{
    this->movement->setMaxSpeed(sf::Vector2f(maxX, maxY));
}

void Enemy::setAIDistances(float aggro, float attack)
{
    this->f_aggroDistance = aggro;
    this->f_attackDistance = attack;
}

void Enemy::setRangedMode(bool ranged)
{
    this->b_isRanged = ranged;
}

void Enemy::setDoPatrol(bool patrol)
{
    this->b_doesPatrol = patrol;
}

void Enemy::overrideHitboxType(HitboxType type)
{
    this->hitbox->setType(type);
}

void Enemy::attack()
{
    if(!b_mutexAttack)
    {
        weapon->startAttack();
    }
}

void Enemy::updateAI()
{
    sf::Vector2f diff = (this->player->getPosition() - this->vf_position);
    float distance = sqrt(diff.x*diff.x + diff.y*diff.y);

    updateAIState(distance, abs(diff.y));

    switch(state) {
        case EnemyAttacking:
            this->b_facingRight = diff.x > 0;
            attack();
            break;
        case EnemyChasing:
            // Move only if there is no
            if((diff.x>0 && this->i_nearPlatformEnd <= 0) || (diff.x<0 && this->i_nearPlatformEnd >= 0))
                this->move(diff.x>0 ? 1 : -1);
            break;
        case EnemyPatrolling:
            if(this->i_nearPlatformEnd > 0 || this->i_wallCollision > 0) b_patrolLeft = true;
            if(this->i_nearPlatformEnd < 0 || this->i_wallCollision < 0) b_patrolLeft = false;

            if(this->b_doesPatrol) this->move(b_patrolLeft ? -1 : 1);
            break;
    }
}

void Enemy::updateAIState(const float& distance, const float& yDiff)
{
    if(distance <= this->f_attackDistance
        && (!this->b_isRanged || (this->b_isRanged && yDiff <= this->getSize().y/2)))
    {
        state = EnemyAttacking;
    }
    else if (distance <= f_aggroDistance)
    {
        state = EnemyChasing;
    }
    else
    {
        state = EnemyPatrolling;
    }
}

void Enemy::die()
{
    this->b_isDead = true;
}

void Enemy::trulyDie()
{
    this->die();
}

void Enemy::linkWorldProjectiles(std::vector<Projectile*>& proyectileArray)
{
    RangedWeapon* ranged = dynamic_cast<RangedWeapon*>(weapon);
    if(ranged) ranged->linkWorldProjectiles(proyectileArray);
}

bool Enemy::checkObstacle(Hitbox* hitbox)
{
    HitboxType type = hitbox->getType();
    bool result = false;
    switch(type)
    {
        case PLATFORM:
        case EXIT:
        result = true; break;
        default: break;
    }

    return result;
}

bool Enemy::checkInteraction(Hitbox* hitbox)
{
    HitboxType type = hitbox->getType();
    bool result = false;
    switch(type)
    {
        case PLAYER_ATTACK:
        case LETHAL:
        result = true; break;
        default: break;
    }

    return result;
}

void Enemy::resizeItems(sf::Vector2f scaleRatio)
{
    this->weapon->scale(scaleRatio);
    this->f_aggroDistance *= scaleRatio.x;
    this->f_attackDistance *= scaleRatio.x;
}

void Enemy::update()
{
    this->updateAI();
    bool finsihedAttacking = this->updateWeapon(this->weapon);
    this->b_mutexAttack = finsihedAttacking;

    // Update general stuff
    this->Entity::update();
}

void Enemy::render(float frameProgress)
{
    this->shape.setPosition(this->getInterpolatedPosition(frameProgress));
    Engine::getInstance()->renderDrawable(&shape);
    this->weapon->render();

    if(false)
    {
        sf::CircleShape aggroArea(this->f_aggroDistance);
        aggroArea.setFillColor(sf::Color(0, 0, 0, 0));
        aggroArea.setOutlineThickness(5);
        aggroArea.setOutlineColor(sf::Color(170, 50, 0));
        aggroArea.setPosition(this->shape.getPosition());
        aggroArea.setOrigin(this->f_aggroDistance,this->f_aggroDistance);

        sf::CircleShape attackArea(this->f_attackDistance);
        attackArea.setFillColor(sf::Color(0, 0, 0, 0));
        attackArea.setOutlineThickness(5);
        attackArea.setOutlineColor(sf::Color(230, 0, 0));
        attackArea.setPosition(this->shape.getPosition());
        attackArea.setOrigin(this->f_attackDistance,this->f_attackDistance);

        Engine* engine = Engine::getInstance();
        engine->renderDrawable(&aggroArea);
        engine->renderDrawable(&attackArea);
    }
}

sf::FloatRect Enemy::getBounds()
{
    return this->shape.getGlobalBounds();
}
