#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "State.h"

class MainMenuState : public State {
private:

public:
    MainMenuState();
    ~MainMenuState();

    void update();
    void render();
};

#endif
