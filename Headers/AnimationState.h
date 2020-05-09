#ifndef ANIMATIONSTATE_H
#define ANIMATIONSTATE_H

#include "State.h"
#include "AnimatedEntity.h"

class AnimationState : public State {
private:
    sf::Vector2f baseResolution;
    float f_animationDuration;
    float f_timeCounter;

    sf::RectangleShape* bgShape;
    std::vector<AnimatedEntity*> v_entities;
public:
    AnimationState(std::stack<State*>* states, Player* player);
    ~AnimationState();

    // Clears the state data. That's just deleting the animated entities
    void clearState();
    // Initializes the state's data using the animation json file stored in Player
    void initState();
    // Returns an index given a predefined game property, and indicating max possible index (min possible index is 0)
    int getGameValue(std::string property, int nCandidates = 1);

    void update();
    void render(float frameProgress);
};

#endif