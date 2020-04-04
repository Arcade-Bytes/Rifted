#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <unordered_map>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>

#include "Animation.h"
#include "ResourceManager.h"

class AnimationComponent
{
private:
    // Player's sprite
    sf::RectangleShape& shape;

    // Map of animations, how many animations will it have
    // Each animation is a vector of frames, and also contains the total duration of itself

    std::unordered_map<std::string, Animation*> animations;
    Animation* current_animation;
public:
    AnimationComponent(sf::RectangleShape& shape);
    ~AnimationComponent();

    void loadAnimationsFromJSON(const std::string& filepath);

    void addAnimation(std::string key, std::vector<Frame*> frames, bool looped = false);
    void playAnimation(const std::string key);
};

#endif
