#include "GameState.h"

GameState::GameState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = GAME_STATE;
    this->level = NULL;
}

GameState::~GameState()
{
    delete this->level;
}

void GameState::initGame()
{
    if(this->level) delete this->level;
    this->level = new Level(player, player->getLevel(), atoi(player->getDoor().c_str()));
}

void GameState::update()
{
    if(this->b_reInit)
    {
        this->b_reInit = false;
        this->initGame();
    }

    this->level->update();

    // Check level change
    if(this->level->didPlayerLeave())
    {
        // Get destination data
        LevelExit* exit = this->level->getActiveExit();
        std::string mapFile = exit->getDestination();
        int entranceIndex = exit->getEntranceIndex();

        // Save game data
        this->player->setLevel(mapFile);
        this->player->setDoor(entranceIndex);
        ftl::SaveGame(*this->player);
        this->level->saveLevelData();

        // Reset level
        delete this->level;
        this->level = new Level(this->player, mapFile, entranceIndex);
    }

    if(Engine::getInstance()->getKeyPressed(sf::Keyboard::P))
    {
        this->changeState(PAUSE_STATE, true);
    }

    StateType nextState = this->level->getNextState();
    if(nextState != GAME_STATE)
    {
        this->level->resetNextState();
        this->changeState(nextState, true);
    }
}

void GameState::render()
{
    if(this->level) this->level->render();
}
