#include "Door.h"

Door::Door(bool isHorizontal)
{
    this->b_isOpen = false;
    this->b_isHorizontal = isHorizontal;

    this->vf_position = {0,0};
    this->hitbox = new Hitbox(PLATFORM, 70,100, vf_position.x, vf_position.y);

    // Rect init
    if(this->b_isHorizontal)
    {
        // Sprite rects for horizontal doors
        doorRects[0] = {96,256,32,64}; // Closed
        doorRects[1] = {96,384,32,64}; // Open
    }
    else
    {
        // Sprite rects for vertical (normal) doors
        doorRects[0] = {192,256,32,64}; // Closed
        doorRects[1] = {192,384,32,64}; // Open
    }

    // Sprite init
    this->shape.setTexture(ResourceManager::getInstance()->loadTexture("resources/door.png"));
    this->shape.setTextureRect(doorRects[0]);
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
    this->shape.setTextureRect(
        b_isOpen ? doorRects[1] : doorRects[0]
    );

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
