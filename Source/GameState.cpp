#include "GameState.h"

GameState::GameState()
{
    this->player = new Player();
    this->level = new Level(player, "maps/Prueba_Beta.tmx", 0);
}

GameState::~GameState()
{
    delete this->player;
    delete this->level;
}

void GameState::update()
{
    this->level->update();

    // Check level change
    if(this->level->didPlayerLeave())
    {
        LevelExit* exit = this->level->getActiveExit();
        std::string mapFile = exit->getDestination();
        int entranceIndex = exit->getEntranceIndex();
        delete this->level;
        this->level = new Level(this->player, mapFile, entranceIndex);
    }
}

void GameState::render()
{
    this->level->render();
}
