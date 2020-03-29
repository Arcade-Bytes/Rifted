#include "GameState.h"

GameState::GameState()
{
    this->player = new Player();
    this->enemy = new Enemy(player);
    this->door = new Door();
    this->lever = new Lever();
    this->lever->addDoor(door);

    this->platform = new Hitbox(PLATFORM, 200,100, 500,400);
    this->platformGround = new Hitbox(PLATFORM, 1500,50, 150,700);
    this->platformWall = new Hitbox(PLATFORM, 50,1500, 50,30);
}

GameState::~GameState()
{
    delete this->player;
    delete this->enemy;
    delete this->door;
    delete this->lever;
    delete this->platform;
    delete this->platformGround;
    delete this->platformWall;
}

void GameState::update()
{
    this->player->update();
    this->enemy->update();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        this->lever->interact();
}

void GameState::render()
{
    this->player->render();
    this->enemy->render();
    this->lever->render();
    this->door->render();
    this->platform->render();
    this->platformGround->render();
    this->platformWall->render();
}
