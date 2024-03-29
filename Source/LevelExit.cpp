#include "LevelExit.h"

LevelExit::LevelExit(std::string destination, const int& entranceIndex)
{
    this->s_destination = destination;
    this->i_entranceIndex = entranceIndex;

    this->hitbox = new Hitbox(EXIT, 0,0, 0,0);
}

LevelExit::~LevelExit()
{
    delete this->hitbox;
}

//Positon related stuff
sf::Vector2f LevelExit::getPosition()
{
    return this->hitbox->getPosition();
}

void LevelExit::setPosition(const float&x, const float& y)
{
    this->hitbox->setPosition(x, y);
}

void LevelExit::setPosition(sf::Vector2f pos)
{
    this->hitbox->setPosition(pos.x, pos.y);
}
//Size setter
void LevelExit::setSize(sf::Vector2f size)
{
    this->hitbox->setSize(size.x, size.y);
}

std::string LevelExit::getDestination()
{
    return this->s_destination;
}

int LevelExit::getEntranceIndex()
{
    return this->i_entranceIndex;
}
//PLayer colison checker
bool LevelExit::checkPlayerCollision(Player* player)
{
    sf::Vector2f collision = this->hitbox->checkCollision(player->getHitbox());
    return collision.x != 0.0f || collision.y != 0.0f;
}

void LevelExit::render()
{
    this->hitbox->render();
}
