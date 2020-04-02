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

    Hitbox* getHitbox();

    void move(const float& xdir);
    void jump(const float& yforce);

    void updateMovement();
    bool updateWeapon(Weapon* weapon);

    /**
     * Returns the current entity's health
     **/
    std::string getHealth();
    /**
     * Returns the maximun health the entity can have
     **/
    std::string getMaxHealth();
    /**
     * Sets the current entity's health
     **/
    void setHealth(float f_health);
    /**
     * Sets the maximun health the entity can have
     **/
    void setMaxHealth(float f_maxHealth);

    void update();
    void render();
};

#endif
