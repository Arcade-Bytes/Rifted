#ifndef HITBOX_H
#define HITBOX_H

#include "Engine.h"

enum HitboxType {PLATFORM = 0, PLAYER, ENEMY, LEVER, DOOR};

class Hitbox {
private:
    // Hitbox lists
    static std::vector<Hitbox*> platformHitboxes;
    static std::vector<Hitbox*> playerHitboxes;
    static std::vector<Hitbox*> enemyHitboxes;
    static std::vector<Hitbox*> leverHitboxes;
    static std::vector<Hitbox*> doorHitboxes;

    HitboxType type;
    sf::Vector2f vf_position;
    sf::Vector2f vf_size;
    sf::RectangleShape shape;
public:
    Hitbox(HitboxType type, const float& xsize, const float& ysize, const float& xpos, const float& ypos);
    ~Hitbox();

    static void resetHitboxLists();
    static std::vector<Hitbox*>* getPlatformHitboxes();
    static std::vector<Hitbox*>* getPlayerHitboxes();
    static std::vector<Hitbox*>* getEnemyHitboxes();
    static std::vector<Hitbox*>* getLeverHitboxes();
    static std::vector<Hitbox*>* getDoorHitboxes();

    sf::Vector2f getPosition();
    void setPosition(const float& x, const float& y);

    sf::Vector2f checkCollision(Hitbox* other);
    sf::FloatRect getBounds();

    void update();
    void render();
};

#endif
