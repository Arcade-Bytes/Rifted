#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "ResourceManager.h"
#include "Hitbox.h"

class Projectile
{
private:
    sf::RectangleShape shape;
    Hitbox* hitbox;

    bool b_isDestroyed;

    sf::Vector2f vf_previousPosition;
    sf::Vector2f vf_nextPosition;
    sf::Vector2f vf_position;
    sf::Vector2f vf_direction; // Valores normalizados, entre -1 y 1
    float f_speed;
    
public:
    Projectile(sf::Vector2f position, sf::Vector2f direction, float speed, float damage, bool isPlayer, sf::Vector2f knockback={0.0f,0.0f});
    ~Projectile();

    /**
     *  Movement controles
     **/
    void move();
    void updateMovement();
    void checkCollisions();
    bool checkInteraction(HitboxType type);
    bool isDestroyed();

    sf::Vector2f getInterpolatedPosition(float frameProgress);
    void updateInterpolationPositions();

    void update();
    void render(float frameProgress);
};

#endif
