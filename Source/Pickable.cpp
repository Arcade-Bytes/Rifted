#include "Pickable.h"

Pickable::Pickable(bool isPicked, std::string newName)
{
    this->b_isPicked = isPicked;
    this->name = newName;
    std::string pickableFile;
    //Created pickable (weapons and coins)
    if(this->name == "Sword")
        pickableFile = "sword_upgrade";
    else if(this->name == "Hammer")
        pickableFile = "hammer";
    else if(this->name == "Bow")
        pickableFile = "bow";
    else if(this->name == "Shield")
        pickableFile = "shield";
    else
        pickableFile = "coin";

    this->shape.setTexture(ResourceManager::getInstance()->loadTexture("resources/"+pickableFile+".png"));
}

Pickable::~Pickable()
{

}
//Position related stuff
sf::Vector2f Pickable::getPosition()
{
    return shape.getPosition();
}

void Pickable::setPosition(sf::Vector2f pos)
{
    this->shape.setPosition(pos);
}
// Size related stuff
sf::Vector2f Pickable::getSize()
{
    return this->shape.getSize();
}

void Pickable::setSize(sf::Vector2f size)
{
    this->shape.setSize(size);
    shape.setOrigin(shape.getSize().x/2.0f, shape.getSize().y/2);
}
//Return if in distance to pick
bool Pickable::isWithinReach(sf::Vector2f pos)
{
    sf::Vector2f diff = pos - this->getPosition();
    float distance = sqrt(diff.x*diff.x + diff.y*diff.y);
    return (distance < this->getSize().x);
}

//Return name
std::string Pickable::getName()
{
    return this->name;
}

//Picking stuff
void Pickable::setIsPicked(bool picked)
{
    this->b_isPicked = picked;
}

bool Pickable::getIsPicked()
{
    return this->b_isPicked;
}

void Pickable::render()
{
    if(!this->b_isPicked) Engine::getInstance()->renderDrawable(&shape);
}
