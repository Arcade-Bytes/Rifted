#ifndef GAME_H
#define GAME_H

#include "Engine.h"
#include "GameState.h"
#include "MainMenuState.h"

class Game {
private:
    State* state;
public:
    Game();
    ~Game();

    void run();
    void updateDelta();
    void updateSFMLEvents();
    void update();
    void render();
};

#endif
