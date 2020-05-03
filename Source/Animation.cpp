#include "Animation.h"

Animation::Animation(sf::RectangleShape& shape, std::vector<Frame*> frames, bool looped)
:shape(shape)
{
    this->isDone = this->isPaused = false;
    this->isLooped = looped;
    this->totalDuration = this->totalElapsed = this->currentFrameIndex = 0;

    for(auto &frame:frames)
    {
        this->addFrame(*frame);
    }
}

Animation::~Animation()
{
    
}

bool Animation::getLooped()
{
    return this->isLooped;
}

bool Animation::getPaused()
{
    return this->isPaused;
}

bool Animation::getDone()
{
    return this->isDone;
}

void Animation::setLooped(bool looped)
{
    this->isLooped = looped;
}

void Animation::setPaused(bool paused)
{
    this->isPaused = paused;
}

void Animation::setDone(bool done)
{
    this->isDone = done;
}

void Animation::addFrame(Frame& frame)
{
    this->totalDuration += frame.duration;
    this->frames.push_back(std::move(frame));
}

void Animation::play(bool mirror)
{
    // TODO::
    if(this->isPaused || this->isDone) return;

    this->totalElapsed += Engine::getInstance()->getDelta() * 1000; // Get it as miliseconds
    
    while(this->totalElapsed >= this->totalDuration)
        this->totalElapsed -= this->totalDuration;

    // Check if the elapsed time of the frame on-screen surpasses it's supposed duration
    while(this->totalElapsed >= this->frames.at(currentFrameIndex).duration)
    {
        this->totalElapsed -= this->frames.at(currentFrameIndex).duration;

        this->currentFrameIndex++;

        if(this->currentFrameIndex >= this->frames.size())
        {
            if(this->isLooped)
            {
                this->currentFrameIndex -= this->frames.size();
            }
            else
            {
                this->isDone = true;
                this->currentFrameIndex = this->frames.size() - 1;
                break;
            }
        }

    }

    float scaleX = this->shape.getScale().x;
    if(mirror)
    {
        scaleX = scaleX > 0 ? -scaleX : scaleX;
    }
    else
    {
        scaleX = scaleX < 0 ? -scaleX : scaleX;
    }
    this->shape.setScale(scaleX, this->shape.getScale().y); // NEW

    this->shape.setTextureRect(this->frames.at(this->currentFrameIndex).rect);
}

void Animation::pause()
{
    this->setPaused(true);
}

void Animation::resume()
{
    this->setPaused(false);
}

void Animation::stop()
{
    this->setDone(true);
}

void Animation::reset()
{
    this->setDone(false);
    this->setPaused(false);
    this->currentFrameIndex = this->totalElapsed = 0;
}

void Animation::skip()
{
    this->currentFrameIndex = this->frames.size()-1;
    this->shape.setTextureRect(this->frames.at(this->currentFrameIndex).rect);
    this->setDone(true);
}
