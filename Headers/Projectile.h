#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Hitbox.h"

class Projectile
{
private:
    // static std::vector<Projectile*> Projectiles;

    sf::RectangleShape shape;
    Hitbox* hitbox;

    bool b_isDestroyed;

    sf::Vector2f vf_position;
    sf::Vector2f vf_direction; // valores normalizados, entre -1 y 1
    float f_speed;
    
public:
    Projectile(sf::Vector2f position, sf::Vector2f direction, float speed, float damage, bool isPlayer, sf::Vector2f knockback={0.0f,0.0f});
    ~Projectile();

    // static std::vector<Projectile*>* getProjectiles();

    void move();
    void updateMovement();
    void checkCollisions();
    bool checkInteraction(HitboxType type);
    bool isDestroyed();

    void update();
    void render();
};

#endif
