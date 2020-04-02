#ifndef GAME_H
#define GAME_H

#define UPDATE_TIME (1.0f/30.0f)

#include "Engine.h"
#include "GameState.h"
#include "MainMenuState.h"

class Game {
private:
    float updateStartTime;

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
