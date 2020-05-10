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

    /**
     * Speed getters and setter
     **/
    sf::Vector2f getSpeed();
    bool isXStopped();
    bool isYStopped();
    void setMaxSpeed(sf::Vector2f maxSpeed);
    /**
     * Movement controler 
     **/
    void move(const float& xdir);
    /**
     * Jump controller
     **/
    void jump(const float& xforce, const float& yforce);
    /**
     * Forcefully stopping movement 
     **/
    void stop();
    void stopX();
    void stopY();
    /**
     * Speed inversion
     **/
    void invertSpeed();
    /**
     * Movements undoner  
     **/
    void undoMove(const float& x, const float& y);

    void resize(sf::Vector2f scaleRatio);

    void update();
    void render();
};

#endif
