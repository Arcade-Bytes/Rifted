#include "GameState.h"

GameState::GameState()
{
    this->map = new Map("maps/cementerio.tmx");

    this->player = new Player();
    this->player->setPosition(
        this->map->getPlayerPosition()
    );

    std::vector<sf::Vector2f> enemyPositions = this->map->getEnemyPositions();
    std::vector<sf::Vector2f> enemySizes = this->map->getEnemySizes();
    std::vector<int> enemyTypes = this->map->getEnemyTypes();
    for(unsigned int i=0; i<enemyPositions.size(); i++)
    {
        Enemy* enemy = new Enemy(this->player);
        enemy->setPosition(enemyPositions[i]);
        enemy->setSize(enemySizes[i]);
        this->enemies.push_back(enemy);
    }

    this->door = new Door();
    this->lever = new Lever();
    this->lever->addDoor(door);
}

GameState::~GameState()
{
    delete this->player;
    for(auto enemy : enemies)
    {
        delete enemy;
        enemy = NULL;
    }
    enemies.clear();
    delete this->door;
    delete this->lever;
}

void GameState::update()
{
    this->player->update();

    for(auto enemy: enemies)
        enemy->update();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        this->lever->interact();
}

void GameState::render()
{
    this->map->render();
    this->player->render();

    for(auto enemy: enemies)
        enemy->render();

    this->lever->render();
    this->door->render();
}
