#include "MovementComponent.h"

MovementComponent::MovementComponent(sf::Vector2f* positionRef, const float& acceleration, const float& deceleration, const float& maxSpeed)
{
    this->vf_position = positionRef;
    this->f_acceleration = acceleration;
    this->f_deceleration = deceleration;
    this->f_maxSpeed = maxSpeed;

    this->f_gravity = 10.0f;
    this->f_gravityModifier = 1.0f;

    this->vf_speed.x = this->vf_speed.y = 0.0f;
}

MovementComponent::~MovementComponent()
{

}

bool MovementComponent::isXStopped()
{
    return (this->vf_speed.x == 0.0f);
}

bool MovementComponent::isYStopped()
{
    return (this->vf_speed.y == 0.0f);
}

void MovementComponent::move(const float& xdir)
{
    float delta = Engine::getInstance()->getDelta();
    this->vf_speed.x += f_acceleration * xdir * delta;
}

void MovementComponent::jump(const float& yforce)
{
    float delta = Engine::getInstance()->getDelta();
    this->vf_speed.y += f_acceleration * -yforce * delta;
}

void MovementComponent::stop()
{
    this->vf_speed = {0,0};
}

void MovementComponent::stopX()
{
    this->vf_speed.x = 0.0f;
}

void MovementComponent::stopY()
{
    this->vf_speed.y = 0.0f;
}

void MovementComponent::undoMove(const float& x, const float& y)
{
    this->vf_position->x -= this->vf_speed.x * x;
    this->vf_position->y -= this->vf_speed.y * y;
}

void MovementComponent::update()
{
    // Get delta value
    float delta = Engine::getInstance()->getDelta();

    if(this->vf_speed.x > 0.0f)
    {
        if(this->vf_speed.x > this->f_maxSpeed) this->vf_speed.x = this->f_maxSpeed;

        this->vf_speed.x -= f_deceleration * delta;

        if(this->vf_speed.x <= 0.0f)            this->vf_speed.x = 0.0f;
    }
    else if(this->vf_speed.x < 0.0f)
    {
        if(this->vf_speed.x < -this->f_maxSpeed) this->vf_speed.x = -this->f_maxSpeed;

        this->vf_speed.x += f_deceleration * delta;
        
        if(this->vf_speed.x >= 0.0f)            this->vf_speed.x = 0.0f;
    }

    if(this->vf_speed.y > this->f_maxSpeed) this->vf_speed.y = this->f_maxSpeed;
    this->vf_speed.y += f_gravity * f_gravityModifier * delta;

    this->vf_position->x += this->vf_speed.x;
    this->vf_position->y += this->vf_speed.y;
}

void MovementComponent::render()
{
    
}
