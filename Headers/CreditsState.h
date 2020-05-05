#ifndef CREDITSSTATE_H
#define CREDITSSTATE_H

#include "State.h"

class CreditsState : public State {
private:
    sf::RectangleShape bgShape;
    std::vector<sf::Text> v_texts;

    float f_creditsSize;
    float f_creditsTime;
    float f_timeCounter;
    sf::Vector2f vf_nextPosition;
    sf::Vector2f vf_prevPosition;
public:
    CreditsState(std::stack<State*>* states, Player* player);
    ~CreditsState();

    void initState();

    void update();
    void render(float frameProgress);
};

#endif