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
    char buffer[65536];
    rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
    rapidjson::Document document;
    document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);

    std::string textureFile = document["texture"].GetString();
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

        this->addAnimation(animations[i]["name"].GetString(), frames, true);
        frames.clear();
    }

    fclose(pFile);

    this->shape.setTexture(ResourceManager::getInstance()->loadTexture("resources/"+textureFile));
}

void AnimationComponent::addAnimation(std::string key, std::vector<Frame*> frames, bool looped)
{
    this->animations[key] = new Animation(this->shape, frames, looped);
}

void AnimationComponent::playAnimation(const std::string key)
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
        this->current_animation->play();
}
