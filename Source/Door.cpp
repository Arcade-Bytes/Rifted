#include "Door.h"

Door::Door()
{
    this->b_isOpen = false;
    this->vf_position = {500,750};
    this->shape.setFillColor(sf::Color::Green);
    this->shape.setSize(sf::Vector2f(70,100));
    this->shape.setOrigin(this->shape.getSize().x/2,this->shape.getSize().y/2);
    this->shape.setPosition(this->vf_position);
}

Door::~Door()
{

}

void Door::toggleOpenState()
{
    this->b_isOpen = !this->b_isOpen;
    this->shape.setFillColor(
        b_isOpen ? sf::Color::Green : sf::Color(0,150,50)
    );
}

void Door::update()
{
 
}

void Door::render()
{
    Engine::getInstance()->renderDrawable(&shape);
}
