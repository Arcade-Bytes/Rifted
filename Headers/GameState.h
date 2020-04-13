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

    void initGame();

    void update();
    void render(float frameProgress);
};

#endif
