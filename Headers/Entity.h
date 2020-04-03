#ifndef ENTITY_H
#define ENTITY_H

#include "Engine.h"
#include "Hitbox.h"
#include "Weapon.h"
#include "RangedWeapon.h"
#include "MovementComponent.h"
#include "AnimationComponent.h"

class Entity {
private:
protected:
    float f_currentHealth;
    float f_maxHealth;
    bool b_isDead;

    bool b_isGrounded;
    bool b_facingRight;

    bool b_isInvulnerable;
    float f_invulnerabilityTime;
    float f_invulnerableBlinkDelta;
    float f_invulnerableBlinkTime;
    bool b_isBlinking;

    int i_wallCollision;
    int i_nearPlatformEnd;

    float f_jumpForce;

    std::string s_currentAnimation;

    sf::Vector2f vf_position;

    Hitbox* hitbox;
    MovementComponent* movement;
    AnimationComponent* animation;

    sf::Texture* spriteTexture;
    sf::RectangleShape shape;
public:
    Entity(const float& maxHealth);
    virtual ~Entity();

    // Position
    sf::Vector2f getPosition();
    void setPosition(const float&x, const float& y);
    void setPosition(sf::Vector2f pos);

    // Size
    void initSize(sf::Vector2f size);
    sf::Vector2f getSize();
    void setSize(sf::Vector2f size);
    virtual void resizeItems(sf::Vector2f scaleRatio) = 0;

    // Weapons
    virtual void linkWorldProjectiles(std::vector<Projectile*>& proyectileArray) = 0;

    // Damage
    void getHurt(float& damage);
    void getHealed(float& healing);
    virtual void die();
    bool isDead();

    // Movement
    void move(const float& xdir);
    void jump(const float& xnormalized, const float& ynormalized);
    void knockback(const float& xforce, const float& yforce);

    // Collisions
    Hitbox* getHitbox();
    void checkCollisions();
    virtual bool checkObstacle(Hitbox* hitbox) = 0;
    virtual bool checkInteraction(Hitbox* hitbox) = 0;

    // Updates
    void updateAnimation();
    void updateMovement();
    void updateInvulnerability();
    bool updateWeapon(Weapon* weapon);

    // Returns the current entity's health
    std::string getHealth();
    // Returns the maximun health the entity can have
    std::string getMaxHealth();
    // Sets the current entity's health
    void setHealth(float f_health);
    // Sets the maximun health the entity can have
    void setMaxHealth(float f_maxHealth);

    void update();
    void render();
};

#endif
