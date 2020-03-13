#include "Lever.h"

Lever::Lever()
{
    v_doorPointers.clear();
    b_isActive = false;

    this->vf_position = {300,750};
    this->shape.setFillColor(sf::Color::Yellow);
    this->shape.setSize(sf::Vector2f(70,70));
    this->shape.setOrigin(this->shape.getSize().x/2,this->shape.getSize().y/2);
    this->shape.setPosition(this->vf_position);
}

Lever::~Lever()
{
    v_doorPointers.clear();
}

void Lever::addDoor(Door* door)
{
    v_doorPointers.push_back(door);
}

void Lever::interact()
{
    b_isActive = !b_isActive;
    for(auto door : v_doorPointers)
    {
        door->toggleOpenState();
    }

    this->shape.setFillColor(
        b_isActive ? sf::Color::Yellow : sf::Color::Blue
    );
}

void Lever::update()
{
 
}

void Lever::render()
{
    Engine::getInstance()->RenderShape(&shape);
}
