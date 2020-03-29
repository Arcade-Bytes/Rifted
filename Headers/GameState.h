#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Lever.h"
#include "Door.h"
#include "BreakableDoor.h"

class GameState : public State {
private:
    Player* player;
    std::vector<Enemy*> enemies;
    Lever *lever;
    Door* door;

    Map* map;
public:
    GameState();
    ~GameState();

    void update();
    void render();
};

#endif
