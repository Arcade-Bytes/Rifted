#include "Enemy.h"

Enemy::Enemy(const float& maxHealth, Player* playerRef)
    : Entity(maxHealth)
{
    this->player = playerRef;
    this->b_patrolLeft = false;
    this->f_aggroDistance = 150;
    this->f_attackDistance = 50;
    this->b_mutexAttack = false;

    this->weapon = new Weapon(0.3f, 0.1f, 0.1f, 40, 60, 10, false);

    this->shape.setFillColor(sf::Color::Red);
}

Enemy::~Enemy()
{
    delete this->weapon;
}

void Enemy::attack()
{
    if(!b_mutexAttack)
        weapon->startAttack();
}

void Enemy::updateAI()
{
    sf::Vector2f diff = (this->player->getPosition() - this->vf_position);
    float distance = sqrt(diff.x*diff.x + diff.y*diff.y);

    updateAIState(distance);

    switch(state) {
        case EnemyAttacking:
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

            this->move(b_patrolLeft ? -1 : 1);
            break;
    }
}

void Enemy::updateAIState(const float& distance)
{
    if(distance <= this->f_attackDistance)
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

bool Enemy::checkObstacle(Hitbox* hitbox)
{
    HitboxType type = hitbox->getType();
    bool result = false;
    switch(type)
    {
        case PLATFORM:
        case BREAKABLE_DOOR:
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

void Enemy::render()
{
    Engine::getInstance()->renderDrawable(&shape);
    this->weapon->render();

    if(false)
    {
        sf::CircleShape aggroArea(this->f_aggroDistance);
        aggroArea.setFillColor(sf::Color(0, 0, 0, 0));
        aggroArea.setOutlineThickness(5);
        aggroArea.setOutlineColor(sf::Color(170, 50, 0));
        aggroArea.setPosition(this->getPosition());
        aggroArea.setOrigin(this->f_aggroDistance,this->f_aggroDistance);

        sf::CircleShape attackArea(this->f_attackDistance);
        attackArea.setFillColor(sf::Color(0, 0, 0, 0));
        attackArea.setOutlineThickness(5);
        attackArea.setOutlineColor(sf::Color(230, 0, 0));
        attackArea.setPosition(this->getPosition());
        attackArea.setOrigin(this->f_attackDistance,this->f_attackDistance);

        Engine* engine = Engine::getInstance();
        engine->renderDrawable(&aggroArea);
        engine->renderDrawable(&attackArea);
    }
}
