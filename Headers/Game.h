#ifndef GAME_H
#define GAME_H

#define UPDATE_TIME (1.0f/15.0f)

#include "Engine.h"
#include "GameState.h"
#include "ShopState.h"
#include "MainMenuState.h"
#include "MenuState.h"
#include "TextState.h"
#include "PauseState.h"
#include "SummaryState.h"
#include "CreditsState.h"

class Game {
private:
    float updateStartTime;
    Player* player;
    std::stack<State*> states;
public:
    Game();
    ~Game();

    void run();
    void updateDelta();
    void updateSFMLEvents();
    void update();
    void render(float frameProgress);
};

#endif
