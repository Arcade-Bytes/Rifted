#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "Map.h"
#include "Hitbox.h"
#include "Player.h"
#include "Enemy.h"
#include "Lever.h"
#include "Door.h"
#include "BreakableDoor.h"

class GameState : public State {
private:
    Player* player;
    Enemy* enemy;
    Lever *lever;
    Door* door;
    Hitbox* platform;
    Hitbox* platformGround;
    Hitbox* platformWall;
public:
    GameState();
    ~GameState();

    void update();
    void render();
};

#endif
