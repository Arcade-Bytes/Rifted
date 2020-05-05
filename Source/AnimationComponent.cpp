#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(sf::RectangleShape& shape)
: shape(shape)
{
    this->current_animation = NULL;
}

AnimationComponent::~AnimationComponent()
{
    for(auto i:animations)
        delete i.second;

    animations.clear();
}

void AnimationComponent::loadAnimationsFromJSON(const std::string& filepath)
{
    // It'll follow a strict format
    FILE* pFile = fopen(filepath.c_str(), "rb");

    if(!pFile) return;

    std::vector<Frame*> frames; // Auxiliar, but careful with the pointers
    bool looped = true; // Default value to pass if it is looped

    char buffer[65536];
    rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
    rapidjson::Document document;
    document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);

    std::string textureFile = document["texture"].GetString();

    float scaleFactor = 1.f;
    if(document.HasMember("scale"))
    {
        scaleFactor = (float)(document["scale"].GetDouble());
        scaleFactor /= 10.0f;
    }

    rapidjson::Value& animations = document["animations"];

    for(unsigned int i = 0; i < animations.Size() ; i++)
    {
        for(unsigned int j = 0; j < animations[i]["frames"].Size(); j++)
        {
            rapidjson::Value& frame = animations[i]["frames"][j];

            frames.push_back(
                new Frame
                (
                    {
                        frame["rect"]["offset_x"].GetInt(),
                        frame["rect"]["offset_y"].GetInt(),
                        frame["rect"]["width"].GetInt(),
                        frame["rect"]["height"].GetInt()
                    },
                    static_cast<float>(frame["duration"].GetInt())
                )
            );
        }

        if(animations[i].HasMember("looped")) looped = animations[i]["looped"].GetBool(); // NEW

        this->addAnimation(animations[i]["name"].GetString(), frames, looped);

        // NEW, reset default aux values
        looped = true;
        frames.clear();
    }

    fclose(pFile);

    this->shape.setTexture(ResourceManager::getInstance()->loadTexture("resources/"+textureFile));
    this->shape.setScale(scaleFactor, scaleFactor);
}

void AnimationComponent::addAnimation(std::string key, std::vector<Frame*> frames, bool looped)
{
    this->animations[key] = new Animation(this->shape, frames, looped);
}

void AnimationComponent::playAnimation(const std::string key, bool mirror)
{
    if(animations.find(key) != animations.end())
    {
        if(!this->current_animation)
        {
            this->current_animation = this->animations[key];
        }

        if(this->current_animation != this->animations[key])
        {
            this->current_animation = this->animations[key];
            this->current_animation->reset();
        }
    }

    if(this->current_animation)
        this->current_animation->play(mirror);
}

void AnimationComponent::stopCurrentAnimation()
{
    if(this->current_animation)
        this->current_animation->stop();
}

void AnimationComponent::resetCurrentAnimation()
{
    if(this->current_animation)
        this->current_animation->reset();
}

void AnimationComponent::skipCurrentAnimation()
{
    if(this->current_animation)
        this->current_animation->skip();
}
