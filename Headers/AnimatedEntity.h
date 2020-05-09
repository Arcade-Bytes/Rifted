#ifndef ANIMATEDENTITY_H
#define ANIMATEDENTITY_H

#include <queue>
#include <math.h>

#include "Engine.h"
#include "AnimationComponent.h"

class AnimatedEntity {
private:
    struct Action {
        float duration;
        sf::Vector2f nextPosition;
        std::string animation;
        bool mirrored;
    };

    sf::Vector2f vf_position;
    sf::Vector2f vf_prevPos;
    sf::Vector2f vf_nextPos;

    sf::RectangleShape* shape;
    AnimationComponent* animator;

    std::queue<Action> actionQueue;

    std::string currentAnimation;
    bool b_isActing;
    float f_actionDelta;
public:
    AnimatedEntity(sf::Vector2f startingPosition, sf::Vector2f size, std::string animationFile);
    ~AnimatedEntity();

    // Add an action to the RC Entity. Indicate animation to play, position to move and time it takes to perform this action
    // An empty animation string means: "Leave previous animation"
    // A NAN value on X or Y axis means: "Stay there!". (NAN,50) means stay where you are in X, but move to 50 in Y
    void addAction(const float& time, sf::Vector2f nextPosition, std::string animation, bool mirrored);
    void adjustActionData();
    void skipToEnd();

    void forceInterpolation();

    void update();
    void render(float frameProgress);
};

#endif
