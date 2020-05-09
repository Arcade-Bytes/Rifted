#include "AnimatedEntity.h"

AnimatedEntity::AnimatedEntity(sf::Vector2f startingPosition, sf::Vector2f size, std::string animationFile)
{
    this->vf_prevPos = this->vf_nextPos = this->vf_position = startingPosition;
    this->vf_prevSize = this->vf_nextSize = this->vf_size = size;
    this->b_isActing = false;
    this->f_actionDelta = 0.0f;

    this->shape = new sf::RectangleShape();
    this->shape->setSize(size);
    this->shape->setOrigin(size/2.0f);
    this->shape->setPosition(this->vf_position);
    this->animator = new AnimationComponent(*shape);
    this->animator->loadAnimationsFromJSON("animations/"+animationFile+".json");
}

AnimatedEntity::~AnimatedEntity()
{
    delete this->shape;
    delete this->animator;
}

void AnimatedEntity::addAction(const float& time, sf::Vector2f nextPosition, sf::Vector2f size, std::string animation, bool mirrored)
{
    Action rcAction;
    rcAction.animation = animation;
    rcAction.nextPosition = nextPosition;
    rcAction.size = size;
    rcAction.duration = time;
    rcAction.mirrored = mirrored;

    actionQueue.push(rcAction);
}

void AnimatedEntity::updateInterpolation()
{
    this->vf_prevPos = this->vf_nextPos;
    this->vf_prevSize = this->vf_nextSize;
}

void AnimatedEntity::update()
{
    this->updateInterpolation();

    if(this->b_isActing)
    {
        Action action = actionQueue.front();
        this->animator->playAnimation(this->currentAnimation, action.mirrored);
        float delta = Engine::getInstance()->getDelta();
        this->f_actionDelta += delta;

        // Calculate how much we advanced in space and volume (size)
        sf::Vector2f distance = action.nextPosition - this->vf_position;
        sf::Vector2f growth = action.size - this->vf_size;
        float progress = this->f_actionDelta / action.duration;
        if(progress > 1.0f) progress = 1.0f;

        this->vf_nextPos = this->vf_position + distance*progress;
        this->vf_nextSize = this->vf_size + growth*progress;

        // Check end of action
        if(this->f_actionDelta >= action.duration)
        {
            this->vf_position = action.nextPosition;
            this->vf_nextPos = vf_position;
            this->vf_size = action.size;
            this->vf_nextSize = vf_size;
            this->b_isActing = false;
            actionQueue.pop();

            if(actionQueue.size() > 0)
            {
                this->currentAnimation = actionQueue.front().animation;

                this->b_isActing = true;
                this->f_actionDelta = 0.0f;
            }
        }
    }
    else
    {
        if(actionQueue.size() > 0)
        {
            this->currentAnimation = actionQueue.front().animation;

            this->b_isActing = true;
            this->f_actionDelta = 0.0f;
        }
    }
}

void AnimatedEntity::render(float frameProgress)
{
    sf::Vector2f pos = this->vf_prevPos + (this->vf_nextPos - this->vf_prevPos) * frameProgress;
    sf::Vector2f size = this->vf_prevSize + (this->vf_nextSize - this->vf_prevSize) * frameProgress;
    this->shape->setSize(size);
    this->shape->setOrigin(size/2.0f);
    this->shape->setPosition(pos);
    Engine::getInstance()->renderDrawable(shape);
}
