#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H

#include "Engine.h"
#include "Hitbox.h"

class MovementComponent {
private:
    float f_acceleration;
    float f_deceleration;
    float f_gravity;
    float f_gravityModifier;
    sf::Vector2f vf_maxSpeed;
    sf::Vector2f vf_speed;
    sf::Vector2f* vf_position;
public:
    MovementComponent(sf::Vector2f* positionRef, const float& acceleration, const float& deceleration, sf::Vector2f maxSpeed);
    ~MovementComponent();

    sf::Vector2f getSpeed();
    bool isXStopped();
    bool isYStopped();

    void move(const float& xdir);
    void jump(const float& xforce, const float& yforce);

    void stop();
    void stopX();
    void stopY();
    void invertSpeed();
    void undoMove(const float& x, const float& y);

    void setMaxSpeed(sf::Vector2f maxSpeed);
    void resize(sf::Vector2f scaleRatio);

    void update();
    void render();
};

#endif
