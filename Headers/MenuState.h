#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "FileManager.h"
#include "State.h"

class MenuState : public State {
private:


    //std::vector<sf::RectangleShape*> botones;

    sf::Sprite* fondoIni;
    sf::Sprite* flecha_selectorR;
    sf::Sprite* flecha_selectorL;
    sf::Texture tex;
    sf::Texture tex2;
    sf::Texture tex3;
    sf::Font fuente;
    sf::Text* texto;
    sf::Clock* clock;

    int i ;
    bool dir; 
    
public:
    MenuState(std::stack<State*>* states, Player* player);
    ~MenuState();

    void update();
    void render();
    void drawText();
    void drawArrow();
    void endState();
};

#endif