#include "Enemy.h"

Enemy::Enemy(Player* playerRef)
    : Entity()
{
    this->player = playerRef;
    this->b_patrolLeft = false;
    this->f_aggroDistance = 300;
    this->f_attackDistance = 50;
    this->b_mutexAttack = false;

    this->weapon = new Weapon(0.3f, 0.1f, 40, 60);

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
            //std::cout << "Attack Attack Attack!!!!" << std::endl;
            attack();
            break;
        case EnemyChasing:
            //std::cout << "Hey youuuuu!!!" << std::endl;
            this->move(diff.x>0 ? 1 : -1);
            break;
        case EnemyPatrolling:
            //std::cout << "Patrolling... ()" << std::endl;
            this->move(b_patrolLeft ? -1 : 1);
            //if(this->movement->isXStopped()) b_patrolLeft = !b_patrolLeft;
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

bool Enemy::isOnPlatform(){}
bool Enemy::isOnPlatform(const float& posx, const float& posy){}
bool Enemy::nextMoveLeavesPlatform(const float& x, const float& y){}

void Enemy::resizeItems(sf::Vector2f scaleRatio)
{
    this->weapon->scale(scaleRatio);
    this->f_aggroDistance *= scaleRatio.x;
    this->f_attackDistance *= scaleRatio.x;
}

void Enemy::update()
{
    this->updateAI();
    this->updateMovement();
    bool finsihedAttacking = this->updateWeapon(this->weapon);
    this->b_mutexAttack = finsihedAttacking;
}

void Enemy::render()
{
    Engine::getInstance()->renderDrawable(&shape);
    this->weapon->render();
}
