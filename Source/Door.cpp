#include "Door.h"

Door::Door()
{
    this->b_isOpen = false;
    this->vf_position = {0,0};
    this->hitbox = new Hitbox(PLATFORM, 70,100, vf_position.x, vf_position.y);

    //this->shape.setFillColor(sf::Color::Green);
    this->shape.setTexture(ResourceManager::getInstance()->loadTexture("resources/door.png"));
    this->shape.setTextureRect(sf::IntRect(192,256,32,64));
    this->setSize(sf::Vector2f(70,100));
    this->shape.setPosition(this->vf_position);
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

    if(b_isOpen)    this->hitbox->setSize(0,0);
    else            this->hitbox->setSize(this->shape.getSize().x, this->shape.getSize().y);
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
    this->shape.setTextureRect(sf::IntRect(
        b_isOpen ? 192 : 192,
        b_isOpen ? 384 : 256,
        32,64
    ));

    this->hitbox->setSize(
        b_isOpen ? 0 : this->shape.getSize().x,
        b_isOpen ? 0 : this->shape.getSize().y
    );
}

void Door::update()
{
 
}

void Door::render()
{
    Engine::getInstance()->renderDrawable(&shape);
}
