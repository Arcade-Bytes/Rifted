#include "GameState.h"

GameState::GameState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = GAME_STATE;
    this->level = NULL;

    this->PocionesShape.setSize({Engine::getInstance()->getBaseResolution().x/18.0f,Engine::getInstance()->getBaseResolution().y/12.0f});
    this->PocionesShape.setOrigin({this->PocionesShape.getSize().x/2.0f,this->PocionesShape.getSize().y/2.0f});
    this->PocionesShape.setPosition({Engine::getInstance()->getBaseResolution().x/24.0f,Engine::getInstance()->getBaseResolution().y/12.0f});

    this->EfectoShape.setSize({Engine::getInstance()->getBaseResolution().x,Engine::getInstance()->getBaseResolution().y});
    this->EfectoShape.setOrigin({this->EfectoShape.getSize().x/2.0f,this->EfectoShape.getSize().y/2.0f});
    this->EfectoShape.setPosition({Engine::getInstance()->getBaseResolution().x/2.0f,Engine::getInstance()->getBaseResolution().y/2.0f});
    this->EfectoShape.setTexture(ResourceManager::getInstance()->loadTexture("resources/GrayScale.png"));
}

GameState::~GameState()
{
    delete this->level;
}

void GameState::initGame()
{
    if(this->level) delete this->level;
    ftl::LoadGame(*this->player);
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
    this->changePotionShape(); 
    this->changeDamageEffects();

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

void GameState::changeDamageEffects() {
    if (this->player->getFloatHealth() <= this->player->getFloatMaxHealth()*0.6f) {
        int opacity = 150 - (this->player->getFloatHealth()*100/this->player->getFloatMaxHealth());
        EfectoShape.setFillColor(sf::Color(255,255,255,(opacity)));
    } else {
        EfectoShape.setFillColor(sf::Color(255,255,255,0));
    }
}

void GameState::changePotionShape() {
    if (this->player->getRemainingPotions() == 0) {
        this->PocionesShape.setTexture(ResourceManager::getInstance()->loadTexture("resources/Empty.png"));
    } else if (this->player->getRemainingPotions() > (this->player->getMaxPotions()/2)) {
        this->PocionesShape.setTexture(ResourceManager::getInstance()->loadTexture("resources/Full-Red.png"));
    } else if (this->player->getRemainingPotions() <= (this->player->getMaxPotions()/2)) {
        this->PocionesShape.setTexture(ResourceManager::getInstance()->loadTexture("resources/Half-Red.png"));
    }
}

void GameState::render(float frameProgress)
{
    if(!this->b_reInit)
    {
        if(this->level) {
            this->level->render(frameProgress);
            Engine::getInstance()->setFollowView(false);
            Engine::getInstance()->renderDrawable(&this->PocionesShape);
            Engine::getInstance()->renderDrawable(&this->EfectoShape);
        }
    }
}
