#include "AnimatedEntity.h"

AnimatedEntity::AnimatedEntity(sf::Vector2f startingPosition, sf::Vector2f size, std::string animationFile)
{
    this->vf_prevPos = this->vf_nextPos = this->vf_position = startingPosition;
    this->b_isActing = false;
    this->f_actionDelta = 0.0f;

    this->shape = new sf::RectangleShape();
    this->shape->setSize(size);
    this->shape->setPosition(this->vf_position);
    this->animator = new AnimationComponent(*shape);
    this->animator->loadAnimationsFromJSON("animations/"+animationFile+".json");
}

AnimatedEntity::~AnimatedEntity()
{
    delete this->shape;
    delete this->animator;
}

void AnimatedEntity::addAction(const float& time, sf::Vector2f nextPosition, std::string animation, bool mirrored)
{
    Action rcAction;
    rcAction.animation = animation;
    rcAction.nextPosition = nextPosition;
    rcAction.duration = time;
    rcAction.mirrored = mirrored;

    actionQueue.push(rcAction);
}

void AnimatedEntity::adjustActionData()
{
    Action action = actionQueue.front();
    this->currentAnimation = action.animation;

    // Adjust action values (for special values)
    if(std::isnan(action.nextPosition.x)) actionQueue.front().nextPosition.x = this->vf_position.x;
    if(std::isnan(action.nextPosition.y)) actionQueue.front().nextPosition.y = this->vf_position.y;
}

void AnimatedEntity::skipToEnd()
{
    this->b_isActing = false;
    while(!actionQueue.empty())
    {
        this->adjustActionData();
        this->vf_position = actionQueue.front().nextPosition;
        this->vf_nextPos = actionQueue.front().nextPosition;
        actionQueue.pop();
    }
    this->vf_prevPos = this->vf_nextPos;
}

void AnimatedEntity::forceInterpolation()
{
    this->vf_prevPos = this->vf_nextPos;
}

void AnimatedEntity::update()
{
    this->vf_prevPos = this->vf_nextPos;

    if(this->b_isActing)
    {
        this->animator->playAnimation(this->currentAnimation);
        Action action = actionQueue.front();
        float delta = Engine::getInstance()->getDelta();
        this->f_actionDelta += delta;

        // Calculate how much we advanced in space
        sf::Vector2f distance = action.nextPosition - this->vf_position;
        float progress = this->f_actionDelta / action.duration;
        if(progress > 1.0f) progress = 1.0f;

        this->vf_nextPos = this->vf_position + distance*progress;

        // Check end of action
        if(this->f_actionDelta >= action.duration)
        {
            this->vf_position = action.nextPosition;
            this->vf_nextPos = action.nextPosition;
            this->b_isActing = false;
            actionQueue.pop();
        }
    }
    else
    {
        if(actionQueue.size() > 0)
        {
            // Adjust next action's wildcards
            this->adjustActionData();

            this->b_isActing = true;
            this->f_actionDelta = 0.0f;
        }
    }
}

void AnimatedEntity::render(float frameProgress)
{
    sf::Vector2f pos = this->vf_prevPos + (this->vf_nextPos - this->vf_prevPos) * frameProgress;
    this->shape->setPosition(pos);
    Engine::getInstance()->renderDrawable(shape);
}
