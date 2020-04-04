#include "Pickable.h"
Pickable::Pickable(bool isPicked)
{
    this->texture.loadFromFile("resources/coin.png");
    this->shape.setTexture(&texture);
    this->name = "";
    this->b_isPicked = isPicked;
}

Pickable::Pickable(bool isPicked, std::string newName)
    : Pickable(isPicked)
{
    this->name = newName;
}

Pickable::~Pickable()
{

}

sf::Vector2f Pickable::getPosition()
{
    return shape.getPosition();
}

void Pickable::setPosition(sf::Vector2f pos)
{
    this->shape.setPosition(pos);
}

sf::Vector2f Pickable::getSize()
{
    return this->shape.getSize();
}

void Pickable::setSize(sf::Vector2f size)
{
    this->shape.setSize(size);
    shape.setOrigin(shape.getSize().x/2.0f, shape.getSize().y/2);
}

bool Pickable::isWithinReach(sf::Vector2f pos)
{
    sf::Vector2f diff = pos - this->getPosition();
    float distance = sqrt(diff.x*diff.x + diff.y*diff.y);
    return (distance < this->getSize().x / 2.0f);
}

std::string Pickable::getName()
{
    return this->name;
}

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
