#include "Hitbox.h"

std::vector<Hitbox*> Hitbox::hitboxes;

Hitbox::Hitbox(HitboxType type, const float& xsize, const float& ysize, const float& xpos, const float& ypos)
{
    this->type = type;
    this->vf_position = {xpos,ypos};
    this->vf_size = {xsize,ysize};

    this->shape.setFillColor(sf::Color(200,200,200));
    this->shape.setSize(this->vf_size);
    this->shape.setOrigin(this->vf_size.x/2,this->vf_size.y/2);
    this->shape.setPosition(this->vf_position);

    hitboxes.push_back(this);
}

Hitbox::~Hitbox()
{
    hitboxes.erase(
        std::find(hitboxes.begin(), hitboxes.end(), this)
    );
}

// Static list management
void Hitbox::resetHitboxLists()
{
    hitboxes.clear();
}

std::vector<Hitbox*>* Hitbox::getAllHitboxes()
{
    return &hitboxes;
}

// Type related
HitboxType Hitbox::getType()
{
    return this->type;
}

// Position related
sf::Vector2f Hitbox::getPosition()
{
    return this->vf_position;
}

void Hitbox::setPosition(const float& x, const float& y)
{
    this->vf_position.x = x;
    this->vf_position.y = y;
    this->shape.setPosition(this->vf_position);
}

// Collision related
sf::Vector2f Hitbox::checkCollision(Hitbox* other)
{
    sf::Vector2f intersection = {0.0f, 0.0f};
    if(this == other) return intersection;
    
    if(this->shape.getGlobalBounds().intersects(other->getBounds()))
    {
        float thisHalfWidth = shape.getGlobalBounds().width / 2.0f;
        float thisHalfHeight = shape.getGlobalBounds().height / 2.0f;
        float otherHalfWidth = other->getBounds().width / 2.0f;
        float otherHalfHeight = other->getBounds().height / 2.0f;

        sf::Vector2f distance = this->getPosition() - other->getPosition();
        intersection = {
            abs(distance.x) - thisHalfWidth - otherHalfWidth,
            abs(distance.y) - thisHalfHeight - otherHalfHeight
        };

        // Checks for most important axis and nullifies the other's effect
        // Also corrects intersection sign for push direction
        if(intersection.x > intersection.y)
        {
            intersection.y = 0.0f;
            if(distance.x > 0) intersection.x *= -1;
        }
        else
        {
            intersection.x = 0.0f;
            if(distance.y > 0) intersection.y *= -1;
        }
    }
    return intersection;
}

sf::FloatRect Hitbox::getBounds()
{
    return this->shape.getGlobalBounds();
}

void Hitbox::update()
{
 
}

void Hitbox::render()
{
    Engine::getInstance()->renderDrawable(&shape);
}
