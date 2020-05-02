#ifndef MainMenuSTATE_H
#define MainMenuSTATE_H

#include "State.h"

class MainMenuState : public State {
private:
    sf::Sprite* fondoIni;
    sf::Text* texto;
    sf::Clock* clock;
    int i ; //Controla la trasparencia del Mensaje
    bool dir; //Controla la direccion de la trasparencia
public:
    MainMenuState(std::stack<State*>* states, Player* player);
    ~MainMenuState();

    void update();
    void render(float frameProgress);
    void endState();
};

#endif