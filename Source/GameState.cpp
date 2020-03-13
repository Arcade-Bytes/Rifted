#include "GameState.h"

GameState::GameState()
{
    this->player = new Player();
    this->enemy = new Enemy(player);
    this->door = new Door();
    this->lever = new Lever();
    this->lever->addDoor(door);
}

GameState::~GameState()
{
    delete this->player;
    delete this->enemy;
    delete this->door;
    delete this->lever;
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
}
