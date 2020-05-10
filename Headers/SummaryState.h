#ifndef SUMMARYSTATE_H
#define SUMMARYSTATE_H

#include "State.h"

class SummaryState : public State {
private:
    sf::RectangleShape bgShape;
    sf::RectangleShape scrollShape;

    std::vector<sf::RectangleShape> v_sprites;
    std::vector<sf::Text> v_texts;
public:
    SummaryState(std::stack<State*>* states, Player* player);
    ~SummaryState();

    /**
     * Initialize state 
    **/

    void initState();

    void update();
    void render(float frameProgress);
};

#endif