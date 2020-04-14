#ifndef TextState_H
#define TextState_H


#include "State.h"

class TextState : public State {
private:
    sf::Text dialogue;
    sf::Sprite* text_box;
    std::vector<std::string> quotes;
    int currentQuote;
public:
    TextState(std::stack<State*>* states, Player* player);
    ~TextState();

    void initConversation();
    void setText(std::string text);
    bool nextQuote();

    void update();
    void render(float frameProgress);
};

#endif