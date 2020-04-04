#ifndef TextState_H
#define TextState_H


#include "State.h"

class TextState : public State {
private:

    sf::Text dialogue;
    sf::Texture box;
    sf::Sprite* text_box;
    sf::Font fuente;

    
public:
    TextState(std::stack<State*>* states, Player* player, sf::Text dialogo);
    ~TextState();

    void update();
    void render();
    void setText(std::string text);
    void endState();
};

#endif