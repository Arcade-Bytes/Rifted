#include "Lever.h"

Lever::Lever()
{
    v_doorPointers.clear();
    b_isActive = false;

    this->vf_position = {300,750};
    this->shape.setTexture(ResourceManager::getInstance()->loadTexture("resources/palanca.png"));
    this->shape.setTextureRect(sf::IntRect(162,163,162,162));
    this->setSize(sf::Vector2f(70,70));
    this->shape.setPosition(this->vf_position);
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

void Lever::addDoor(Door* door)
{
    v_doorPointers.push_back(door);
}

bool Lever::getIsActive()
{
    return this->b_isActive;
}

void Lever::interact()
{
    b_isActive = !b_isActive;
    for(auto door : v_doorPointers)
    {
        door->toggleOpenState();
    }

    this->shape.setTextureRect(sf::IntRect(
        b_isActive ? 0 : 162,
        b_isActive ? 0 : 163,
        162,162
    ));
}

void Lever::update()
{
 
}

void Lever::render()
{
    Engine::getInstance()->renderDrawable(&shape);
}
