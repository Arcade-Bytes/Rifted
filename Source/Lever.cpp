#include "Lever.h"

Lever::Lever()
{
    v_doorPointers.clear();
    b_isActive = false;

    this->vf_position = {300,750};
    this->shape.setTexture(ResourceManager::getInstance()->loadTexture("resources/Palancas.png"));
    this->shape.setTextureRect(sf::IntRect(0,77,89,77));
    this->setSize(sf::Vector2f(70,70));
    this->shape.setPosition(this->vf_position);
    toggleClock.restart();
}

Lever::~Lever()
{
    v_doorPointers.clear();
}

sf::Vector2f Lever::getPosition()
{
    return this->vf_position;
}

void Lever::setPosition(const float&x, const float& y)
{
    this->vf_position = {x, y};
    this->shape.setPosition(this->vf_position);
}

void Lever::setPosition(sf::Vector2f pos)
{
    this->vf_position = pos;
    this->shape.setPosition(this->vf_position);
}

sf::Vector2f Lever::getSize()
{
    return this->shape.getSize();
}

void Lever::setSize(sf::Vector2f size)
{
    this->shape.setSize(size);
    this->shape.setOrigin(this->shape.getSize().x/2,this->shape.getSize().y/2);
}
// Link door to lever
void Lever::addDoor(Door* door)
{
    v_doorPointers.push_back(door);
}

bool Lever::getIsActive()
{
    return this->b_isActive;
}

//Changes sprites of door and lever depending on state
void Lever::interact(bool playSound)
{
    b_isActive = !b_isActive;
    if(playSound)
    {
        ResourceManager::getInstance()->playSound("lever_pull");
        if(b_isActive)
            ResourceManager::getInstance()->playSound("door_action");
        else
            ResourceManager::getInstance()->playSound("door_action_deep");
    }
    
    
    for(auto door : v_doorPointers)
    {
        door->toggleOpenState();
    }

    this->shape.setTextureRect(sf::IntRect(
        0,
        b_isActive ? 0 : 77,
        89,77
    ));
}

void Lever::update()
{
 
}

void Lever::render()
{
    Engine::getInstance()->renderDrawable(&shape);
}

float Lever::getToggleTime(){

    return toggleClock.getElapsedTime().asSeconds();

}
void Lever::restartToggleTime(){

    toggleClock.restart();

}