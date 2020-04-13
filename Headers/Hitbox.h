#ifndef HITBOX_H
#define HITBOX_H

#include "Engine.h"

enum HitboxType {NO_COLLISION = 0, PLATFORM, PLAYER, ENEMY, LETHAL, PLAYER_ATTACK, ENEMY_ATTACK, LEVER, EXIT};
enum DamageType {LIGHT_ATTACK, HEAVY_ATTACK, RANGED_ATTACK};

class Hitbox {
private:
    // Hitbox lists
    static std::vector<Hitbox*> hitboxes;

    // Game data
    float f_damage;
    sf::Vector2f vf_knockback;
    DamageType damageType;
    HitboxType type;

    sf::Vector2f vf_position;
    sf::Vector2f vf_size;
    sf::RectangleShape shape;
public:
    Hitbox(HitboxType type, const float& xsize, const float& ysize, const float& xpos, const float& ypos,
        float damage = 0.0f, sf::Vector2f knockback = {0.0f,0.0f}, DamageType damageTypeVal = LIGHT_ATTACK);
    ~Hitbox();

    // Static list management
    static void resetHitboxLists();
    static std::vector<Hitbox*>* getAllHitboxes();

    // Game related
    void setType(HitboxType type);
    HitboxType getType();
    void setDamage(float damage);
    float getDamage();
    void setKnockback(sf::Vector2f newKnockback);
    sf::Vector2f getKnockback();
    DamageType getDamageType();

    // Position related
    sf::Vector2f getPosition();
    void setPosition(const float& x, const float& y);
    sf::Vector2f getSize();
    void setSize(const float& x, const float& y);
    void scale(sf::Vector2f scaleRatio);

    // Collision related
    bool checkBooleanCollision(Hitbox* other);
    sf::Vector2f checkCollision(Hitbox* other);
    sf::FloatRect getBounds();

    // Visual related
    void setColor(sf::Color color);

    void update();
    void render();
};

#endif
