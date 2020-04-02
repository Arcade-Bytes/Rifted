#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "Level.h"

class GameState : public State {
private:
    Level* level;
public:
    GameState(std::stack<State*>* states, Player* player);
    ~GameState();

    void update();
    void render();
};

#endif
