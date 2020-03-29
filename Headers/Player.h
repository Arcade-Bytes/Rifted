#ifndef PLAYER_H
#define PLAYER_H

#include "Engine.h"
#include "Entity.h"
#include "Weapon.h"
#include "Shield.h"

class Player : public Entity {
private:
    // Life regeneration properties
    float f_regenerationDelta;      // Time since last damage/healing action
    float f_regenerationCooldown;   // Necessary time to start regenerating
    float f_regenerationSpeed;      // Time between regen and regen once you're already regenerating
    float f_regenerationAmount;     // Amount of life regenerated every update
    int i_nchunks;                  // Number of chunks the lifebar is divided in

    Weapon* sword;
    Weapon* hammer;
    Shield* shield;
public:
    Player();
    ~Player();

    void getHurt(float& damage);
    void getHealed(float& healing);
    void regenerate();

    void update();
    void render();
};

#endif
