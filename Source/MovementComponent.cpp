#include "MovementComponent.h"

MovementComponent::MovementComponent(sf::Vector2f* positionRef, const float& acceleration, const float& deceleration, sf::Vector2f maxSpeed)
{
    this->vf_position = positionRef;
    this->f_acceleration = acceleration;
    this->f_deceleration = deceleration;
    this->vf_maxSpeed = maxSpeed;

    this->f_gravity = 1200.0f;
    this->f_gravityModifier = 1.0f;

    this->vf_speed = {0.0f, 0.0f};
}

MovementComponent::~MovementComponent()
{

}

sf::Vector2f MovementComponent::getSpeed()
{
    return this->vf_speed;
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
    this->vf_speed.x += f_acceleration * xdir;
}

void MovementComponent::jump(const float& xforce, const float& yforce)
{
    this->vf_speed.x += -xforce;
    this->vf_speed.y += -yforce;
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

void MovementComponent::invertSpeed()
{
    this->vf_speed.x *= -1;
    this->vf_speed.y *= -1;
}

void MovementComponent::undoMove(const float& x, const float& y)
{
    float delta = Engine::getInstance()->getDelta();
    this->vf_position->x -= this->vf_speed.x * x * delta;
    this->vf_position->y -= this->vf_speed.y * y * delta;
}

void MovementComponent::setMaxSpeed(sf::Vector2f maxSpeed)
{
    this->vf_maxSpeed = maxSpeed;
}

void MovementComponent::resize(sf::Vector2f scaleRatio)
{
    this->f_acceleration *= scaleRatio.x;
    this->f_deceleration *= scaleRatio.x;
    //this->vf_maxSpeed.x *= scaleRatio.x;
    this->vf_maxSpeed.y *= scaleRatio.y;
    this->f_gravity *= scaleRatio.y;
}

void MovementComponent::update()
{
    // Get delta value
    float delta = Engine::getInstance()->getDelta();

    if(this->vf_speed.x > 0.0f)
    {
        // Clamp X speed to its max, substract deceleration, clamp to 0
        if(this->vf_speed.x > this->vf_maxSpeed.x) this->vf_speed.x = this->vf_maxSpeed.x;
        this->vf_speed.x -= f_deceleration * delta;
        if(this->vf_speed.x <= 0.0f)            this->vf_speed.x = 0.0f;
    }
    else if(this->vf_speed.x < 0.0f)
    {
        // Clamp X speed to its min, substract deceleration, clamp to 0
        if(this->vf_speed.x < -this->vf_maxSpeed.x) this->vf_speed.x = -this->vf_maxSpeed.x;
        this->vf_speed.x += f_deceleration * delta;
        if(this->vf_speed.x >= 0.0f)            this->vf_speed.x = 0.0f;
    }

    // Gravity is stronger when going down, because it looks better
    if(this->vf_speed.y > 0.0f) this->vf_speed.y += 2.0f * f_gravity * f_gravityModifier * delta;
    else                        this->vf_speed.y += f_gravity * f_gravityModifier * delta;

    // Clamp Y speed
    if(this->vf_speed.y > this->vf_maxSpeed.y) this->vf_speed.y = this->vf_maxSpeed.y;
    if(this->vf_speed.y < -this->vf_maxSpeed.y) this->vf_speed.y = -this->vf_maxSpeed.y;

    this->vf_position->x += this->vf_speed.x * delta;
    this->vf_position->y += this->vf_speed.y * delta;
}

void MovementComponent::render()
{
    
}
