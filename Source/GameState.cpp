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
    ResourceManager::getInstance()->stopMainMenu();                             //stop the main menu music
    ResourceManager::getInstance()->playLevelMusic(this->player->getLevel());   //play the main level music
    if(this->level) delete this->level;
    ftl::LoadGame(*this->player);
    this->level = new Level(player, player->getLevel(), atoi(player->getDoor().c_str()));
    
}

void GameState::update()
{
    ResourceManager::getInstance()->musicUpdate();  //update the music, loops and stuff
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

    // Pause game
    if(Engine::getInstance()->getKeyPressed(sf::Keyboard::P) || Engine::getInstance()->getKeyPressed(sf::Keyboard::Escape))
    {
        this->level->forceInterpolationUpdate();
        this->changeState(PAUSE_STATE, true);
    }

    // Change state
    StateType nextState = this->level->getNextState();
    if(nextState != GAME_STATE)
    {
        this->level->resetNextState();
        this->changeState(nextState, true);
    }

    // Reset level, load last save file
    if(this->level->didPlayerDie())
    {
        this->b_reInit = true;
    }
}

void GameState::render(float frameProgress)
{
    if(!this->b_reInit)
    {
        if(this->level) this->level->render(frameProgress);
    }
}
