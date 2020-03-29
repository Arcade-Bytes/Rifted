#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "Level.h"

class GameState : public State {
private:
    Player* player;
    Level* level;
public:
    GameState();
    ~GameState();

    void update();
    void render();
};

#endif
