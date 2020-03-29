#include "Door.h"

Door::Door()
{
    this->b_isOpen = false;
    this->vf_position = {0,0};
    this->shape.setFillColor(sf::Color::Green);
    this->shape.setSize(sf::Vector2f(70,100));
    this->shape.setOrigin(this->shape.getSize().x/2,this->shape.getSize().y/2);
    this->shape.setPosition(this->vf_position);

    this->hitbox = new Hitbox(PLATFORM, 70,100, vf_position.x, vf_position.y);
}

Door::~Door()
{
    delete this->hitbox;
}

sf::Vector2f Door::getPosition()
{
    return this->vf_position;
}

void Door::setPosition(const float&x, const float& y)
{
    this->vf_position = {x, y};
    this->shape.setPosition(this->vf_position);
    this->hitbox->setPosition(this->vf_position.x, this->vf_position.y);
}

void Door::setPosition(sf::Vector2f pos)
{
    this->vf_position = pos;
    this->shape.setPosition(this->vf_position);
    this->hitbox->setPosition(this->vf_position.x, this->vf_position.y);
}

void Door::setSize(sf::Vector2f size)
{
    this->shape.setSize(size);
    this->shape.setOrigin(this->shape.getSize().x/2,this->shape.getSize().y/2);
    this->hitbox->setSize(this->shape.getSize().x, this->shape.getSize().y);
}

void Door::setVinculationId(std::string id)
{
    this->s_vinculationId = id;
}

std::string Door::getVinculationId()
{
    return this->s_vinculationId;
}

void Door::toggleOpenState()
{
    this->b_isOpen = !this->b_isOpen;
    this->shape.setFillColor(
        b_isOpen ? sf::Color::Green : sf::Color(0,150,50)
    );

    this->hitbox->setSize(
        b_isOpen ? this->shape.getSize().x : 0,
        b_isOpen ? this->shape.getSize().y : 0
    );
}

void Door::update()
{
 
}

void Door::render()
{
    Engine::getInstance()->renderDrawable(&shape);
}
