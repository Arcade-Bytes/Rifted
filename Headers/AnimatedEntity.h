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
        sf::Vector2f size;
        std::string animation;
        bool mirrored;
    };

    sf::Vector2f vf_position;
    sf::Vector2f vf_prevPos;
    sf::Vector2f vf_nextPos;

    sf::Vector2f vf_size;
    sf::Vector2f vf_prevSize;
    sf::Vector2f vf_nextSize;

    sf::RectangleShape* shape;
    AnimationComponent* animator;

    std::queue<Action> actionQueue;

    std::string currentAnimation;
    bool b_isActing;
    float f_actionDelta;
public:
    AnimatedEntity(sf::Vector2f startingPosition, sf::Vector2f size, std::string animationFile);
    ~AnimatedEntity();

    // Add an action to action queue of this entity, indicating duration of the action, position it moves to, size it changes to, animation to play, and if it's mirrored
    void addAction(const float& time, sf::Vector2f nextPosition, sf::Vector2f size, std::string animation, bool mirrored);
    // Assigns the next interpolated position to the previous one to synchronize them
    void updateInterpolation();

    void update();
    void render(float frameProgress);
};

#endif
