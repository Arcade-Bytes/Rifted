#ifndef HITBOX_H
#define HITBOX_H

#include "Engine.h"

enum HitboxType {NO_COLLISION = 0, PLATFORM, PLAYER, ENEMY, LETHAL, PLAYER_ATTACK, ENEMY_ATTACK, LEVER, BREAKABLE_DOOR, EXIT};

class Hitbox {
private:
    // Hitbox lists
    static std::vector<Hitbox*> hitboxes;

    // Game data
    float f_damage;
    HitboxType type;

    sf::Vector2f vf_position;
    sf::Vector2f vf_size;
    sf::RectangleShape shape;
public:
    Hitbox(HitboxType type, const float& xsize, const float& ysize, const float& xpos, const float& ypos);
    Hitbox(HitboxType type, const float& xsize, const float& ysize, const float& xpos, const float& ypos, float damage);
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
    void setDamage(float damage);
    float getDamage();

    // Visual related
    void setColor(sf::Color color);

    void update();
    void render();
};

#endif
