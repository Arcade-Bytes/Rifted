#ifndef HITBOX_H
#define HITBOX_H

#include "Engine.h"

enum HitboxType {NO_COLLISION = 0, PLATFORM, PLAYER, ENEMY, WEAPON, LEVER, BREAKABLE_DOOR, PROJECTILE};

class Hitbox {
private:
    // Hitbox lists
    static std::vector<Hitbox*> hitboxes;

    HitboxType type;
    sf::Vector2f vf_position;
    sf::Vector2f vf_size;
    sf::RectangleShape shape;
public:
    Hitbox(HitboxType type, const float& xsize, const float& ysize, const float& xpos, const float& ypos);
    ~Hitbox();

    // Static list management
    static void resetHitboxLists();
    static std::vector<Hitbox*>* getAllHitboxes();

    // Type related
    HitboxType getType();

    // Position related
    sf::Vector2f getPosition();
    void setPosition(const float& x, const float& y);
    sf::Vector2f getSize();
    void setSize(const float& x, const float& y);
    void scale(sf::Vector2f scaleRatio);

    // Collision related
    sf::Vector2f checkCollision(Hitbox* other);
    sf::FloatRect getBounds();

    // Visual related
    void setColor(sf::Color color);

    void update();
    void render();
};

#endif
