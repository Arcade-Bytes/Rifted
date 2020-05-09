#ifndef ANIMATIONSTATE_H
#define ANIMATIONSTATE_H

#include "State.h"
#include "AnimatedEntity.h"

class AnimationState : public State {
private:
    sf::Vector2f baseResolution;
    sf::RectangleShape* bgShape;
    std::vector<AnimatedEntity*> v_entities;
public:
    AnimationState(std::stack<State*>* states, Player* player);
    ~AnimationState();

    void initState();

    int getGameValue(std::string property, int nCandidates = 1);

    void update();
    void render(float frameProgress);
};

#endif