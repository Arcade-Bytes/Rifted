#ifndef ANIMATION_H
#define ANIMATION_H

#include "Engine.h"

struct Frame {
   sf::IntRect rect;
   float duration; // in seconds

   Frame(sf::IntRect rect, float duration)
   {
       this->rect = rect;
       this->duration = duration;
   }
};

class Animation
{
private:
    std::vector<Frame> frames;
    float totalDuration;
    float totalElapsed;
    unsigned int currentFrameIndex;

    bool isLooped;
    bool isPaused;
    bool isDone;

    // Reference to the shape
    sf::RectangleShape& shape;
public:
    Animation(sf::RectangleShape& shape, std::vector<Frame*> frames, bool looped = false);
    ~Animation();


    bool getLooped();
    bool getPaused();
    bool getDone();

    void setLooped(bool looped);
    void setPaused(bool paused);
    void setDone(bool done);

    void addFrame(Frame& frame);


    void play();
    void pause();
    void resume();
    void stop();
    void reset();
};

#endif
