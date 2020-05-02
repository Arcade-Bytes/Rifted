#ifndef MainMenuSTATE_H
#define MainMenuSTATE_H

#include "State.h"

class MainMenuState : public State {
private:
    sf::RectangleShape* fondoIni;
    sf::Text* textoTitle;
    sf::Text* textoPressEnter;
    sf::Clock* clock;
    int alpha ; //Controla la trasparencia del Mensaje
    int prevAlpha;
    bool dir; //Controla la direccion de la trasparencia
public:
    MainMenuState(std::stack<State*>* states, Player* player);
    ~MainMenuState();

    void update();
    void render(float frameProgress);
    void endState();
};

#endif