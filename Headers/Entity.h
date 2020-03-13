#ifndef ENTITY_H
#define ENTITY_H

#include "Engine.h"
#include "Weapon.h"
#include "MovementComponent.h"

class Entity {
private:
protected:
    float f_currentHealth;
    float f_maxHealth;

    sf::Vector2f vf_position;
    MovementComponent* movement;
    sf::RectangleShape shape;
public:
    Entity();
    ~Entity();

    // Accesors
    sf::Vector2f getPosition();

    void move(const float& xdir);
    void jump(const float& yforce);

    void updateMovement();
    bool updateWeapon(Weapon* weapon);

    void update();
    void render();
};

#endif
