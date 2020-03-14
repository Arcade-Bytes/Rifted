#include "Hitbox.h"

std::vector<Hitbox*> Hitbox::platformHitboxes;
std::vector<Hitbox*> Hitbox::playerHitboxes;
std::vector<Hitbox*> Hitbox::enemyHitboxes;
std::vector<Hitbox*> Hitbox::leverHitboxes;
std::vector<Hitbox*> Hitbox::doorHitboxes;

Hitbox::Hitbox(HitboxType type, const float& xsize, const float& ysize, const float& xpos, const float& ypos)
{
    this->type = type;
    this->vf_position = {xpos,ypos};
    this->vf_size = {xsize,ysize};

    this->shape.setFillColor(sf::Color(200,200,200));
    this->shape.setSize(this->vf_size);
    this->shape.setOrigin(this->vf_size.x/2,this->vf_size.y/2);
    this->shape.setPosition(this->vf_position);

    switch (type)
    {
    case PLATFORM:  platformHitboxes.push_back(this);   break;
    case PLAYER:    playerHitboxes.push_back(this);     break;
    case ENEMY:     enemyHitboxes.push_back(this);      break;
    case LEVER:     leverHitboxes.push_back(this);      break;
    case DOOR:      doorHitboxes.push_back(this);       break;
    default: break;
    }
}

Hitbox::~Hitbox()
{
    switch (type)
    {
    case PLATFORM:
        platformHitboxes.erase(
            std::find(platformHitboxes.begin(), platformHitboxes.end(), this)
        );
        break;
    case PLAYER:
        playerHitboxes.erase(
            std::find(playerHitboxes.begin(), playerHitboxes.end(), this)
        );
        break;
    case ENEMY:
        enemyHitboxes.erase(
            std::find(enemyHitboxes.begin(), enemyHitboxes.end(), this)
        );
        break;
    case LEVER:
        leverHitboxes.erase(
            std::find(leverHitboxes.begin(), leverHitboxes.end(), this)
        );
        break;
    case DOOR:
        doorHitboxes.erase(
            std::find(doorHitboxes.begin(), doorHitboxes.end(), this)
        );
        break;
    }
}

sf::Vector2f Hitbox::checkCollision(Hitbox* other)
{
    sf::Vector2f intersection = {0.0f, 0.0f};
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

        // Checks for most important axis and nullifies the others offect
        // Also corrects intersction sign for push direction
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

void Hitbox::resetHitboxLists()
{
    platformHitboxes.clear();
    playerHitboxes.clear();
    enemyHitboxes.clear();
    leverHitboxes.clear();
    doorHitboxes.clear();
}

std::vector<Hitbox*>* Hitbox::getPlatformHitboxes() { return &platformHitboxes; }
std::vector<Hitbox*>* Hitbox::getPlayerHitboxes()   { return &playerHitboxes; }
std::vector<Hitbox*>* Hitbox::getEnemyHitboxes()    { return &enemyHitboxes; }
std::vector<Hitbox*>* Hitbox::getLeverHitboxes()    { return &leverHitboxes; }
std::vector<Hitbox*>* Hitbox::getDoorHitboxes()     { return &doorHitboxes; }

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

void Hitbox::update()
{
 
}

void Hitbox::render()
{
    Engine::getInstance()->RenderShape(&shape);
}
