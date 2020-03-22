#ifndef HITBOX_H
#define HITBOX_H

#include "Engine.h"

enum HitboxType {PLATFORM = 0, PLAYER, ENEMY, LEVER, BREAKABLE_DOOR, ARROW};

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

    // Collision related
    sf::Vector2f checkCollision(Hitbox* other);
    sf::FloatRect getBounds();

    void update();
    void render();
};

#endif
