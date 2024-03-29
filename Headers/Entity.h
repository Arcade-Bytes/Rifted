#ifndef ENTITY_H
#define ENTITY_H

#include "Engine.h"
#include "ResourceManager.h"
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

    bool b_mutexAttack;

    bool b_isInvulnerable;
    float f_invulnerabilityTime;
    float f_invulnerableBlinkDelta;
    float f_invulnerableBlinkTime;
    bool b_isBlinking;

    int i_wallCollision;
    int i_nearPlatformEnd;

    float f_jumpForce;

    std::map<DamageType, float> resistances;

    std::string s_currentAnimation;

    sf::Vector2f vf_previousPosition;
    sf::Vector2f vf_nextPosition;
    sf::Vector2f vf_position;

    Hitbox* hitbox;
    Hitbox* collisionArea;
    int collisionsCalculated;
    MovementComponent* movement;
    AnimationComponent* animation;

    sf::RectangleShape shape;
public:
    Entity(const float& maxHealth);
    virtual ~Entity();

    // Position
    sf::Vector2f getPosition();
    void setPosition(const float&x, const float& y);
    void setPosition(sf::Vector2f pos);
    void initPosition(sf::Vector2f pos);

    // Size
    void initSize(sf::Vector2f size);
    sf::Vector2f getSize();
    void setSize(sf::Vector2f size);
    virtual void resizeItems(sf::Vector2f scaleRatio) = 0;

    // Weapons
    virtual void linkWorldProjectiles(std::vector<Projectile*>& proyectileArray) = 0;

    // Damage
    void setResistances(float lightRes, float heavyRes, float rangedRes);
    float getResistance(DamageType type);
    virtual float getHurt(float& damage);
    virtual float getHealed(float& healing);
    // Meant to be overrided: What this entity does when its life points reach 0
    virtual void die();
    // Meant to be overrided: What this entity does when it hits a Lethal Area (Hitbox type / Map area)
    virtual void trulyDie();
    void revive();

    bool isDead();

    // Movement
    void move(const float& xdir);
    void jump(const float& xnormalized, const float& ynormalized);
    void knockback(const float& xforce, const float& yforce);
    void stopSpeed();

    // Collisions
    Hitbox* getHitbox();
    void checkCollisions();
    bool checkObstacleCollision(Hitbox* hitbox);
    bool checkInteractionCollision(Hitbox* hitbox);
    void calculateCollisionArea();
    virtual bool checkObstacle(Hitbox* hitbox) = 0;
    virtual bool checkInteraction(Hitbox* hitbox) = 0;

    // Updates
    virtual void updateAnimation();
    virtual void updateMovement();
    void updateInvulnerability();
    bool updateWeapon(Weapon* weapon);

    // Returns the current entity's health
    std::string getHealth();
    float getFloatHealth();
    // Returns the maximun health the entity can have
    std::string getMaxHealth();
    float getFloatMaxHealth();
    // Sets the current entity's health
    void setHealth(float f_health);
    // Sets the maximun health the entity can have
    void setMaxHealth(float f_maxHealth);

    void update();
    void render(float frameProgress);

    // Interpolation related
    sf::Vector2f getInterpolatedPosition(float frameProgress);
    void updateInterpolationPositions();
};

#endif
