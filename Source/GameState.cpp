#include "GameState.h"

GameState::GameState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = GAME_STATE;
    this->level = new Level(player, "Prueba_Beta", 0);
}

GameState::~GameState()
{
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

    if(Engine::getInstance()->getKeyPressed(sf::Keyboard::P))
    {
        this->changeState(PAUSE_STATE);
    }

    StateType nextState = this->level->getNextState();
    if(nextState != GAME_STATE)
    {
        this->level->resetNextState();
        this->changeState(nextState);
    }
}

void GameState::render()
{
    this->level->render();
}
