#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "FileManager.h"
#include "State.h"

class MenuState : public State {
private:


    //std::vector<sf::RectangleShape*> botones;

    sf::RectangleShape* fondoIni;
    sf::Sprite* flecha_selectorR;
    sf::Sprite* flecha_selectorL;
    sf::Text* texto;
    sf::Clock* clock;

    int arrowMovement;
    int prevArrowMovement;
    bool dir; 
public:
    MenuState(std::stack<State*>* states, Player* player);
    ~MenuState();

    void update();
    void render(float frameProgress);
    void drawText();
    void drawArrow(float frameProgress);
};

#endif