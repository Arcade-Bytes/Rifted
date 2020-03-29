#ifndef ENTITY_H
#define ENTITY_H

#include "Engine.h"
#include "Hitbox.h"
#include "Weapon.h"
#include "MovementComponent.h"

class Entity {
private:
protected:
    float f_currentHealth;
    float f_maxHealth;

    bool b_isGrounded;

    Hitbox* hitbox;

    sf::Vector2f vf_position;
    MovementComponent* movement;
    sf::RectangleShape shape;
public:
    Entity();
    ~Entity();

    // Accesors
    sf::Vector2f getPosition();
    void setPosition(const float&x, const float& y);
    void setPosition(sf::Vector2f pos);
    void setSize(sf::Vector2f size);

    void move(const float& xdir);
    void jump(const float& yforce);

    void updateMovement();
    bool updateWeapon(Weapon* weapon);

    void update();
    void render();
};

#endif
