#ifndef TextState_H
#define TextState_H


#include "State.h"

class TextState : public State {
private:
    sf::Text dialogue;
    sf::RectangleShape* text_box;
    std::vector<std::string> quotes;
    int currentQuote;
public:
    TextState(std::stack<State*>* states, Player* player);
    ~TextState();
    /**
     * Takes the dialogue from the Nearest NPC
     **/
    void initConversation();
    /**
     * Set dialogue to text 
     **/
    void setText(std::string text);
    /**
     * Sets dialogue to the next line of quotes
     **/
    bool nextQuote();

    void update();
    void render(float frameProgress);
};

#endif