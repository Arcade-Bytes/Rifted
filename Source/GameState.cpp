#include "GameState.h"

GameState::GameState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = GAME_STATE;
    this->level = NULL;

    this->PocionesShape.setSize({Engine::getInstance()->getBaseResolution().x/18.0f,Engine::getInstance()->getBaseResolution().y/12.0f});
    this->PocionesShape.setOrigin({this->PocionesShape.getSize().x/2.0f,this->PocionesShape.getSize().y/2.0f});
    this->PocionesShape.setPosition({Engine::getInstance()->getBaseResolution().x/24.0f,Engine::getInstance()->getBaseResolution().y/12.0f});
    this->changePotionShape();

    this->EfectoShape.setSize({(float)Engine::getInstance()->getBaseResolution().x,(float)Engine::getInstance()->getBaseResolution().y});
    this->EfectoShape.setOrigin({this->EfectoShape.getSize().x/2.0f,this->EfectoShape.getSize().y/2.0f});
    this->EfectoShape.setPosition({Engine::getInstance()->getBaseResolution().x/2.0f,Engine::getInstance()->getBaseResolution().y/2.0f});
    this->EfectoShape.setTexture(ResourceManager::getInstance()->loadTexture("resources/GrayScale.png"));
    this->EfectoShape.setFillColor(sf::Color(255,255,255,0));

    // Transition init
    this->f_transitionTime = 1.0f;
    this->f_transitionCounter = 0.0f;
    this->b_isTransitioning = true;
    this->b_isLeaving = false;

    this->fadeOutPanel.setSize({
        (float)Engine::getInstance()->getBaseResolution().x,
        (float)Engine::getInstance()->getBaseResolution().y
    });
    this->fadeOutPanel.setFillColor(sf::Color(0,0,0,255));
}

GameState::~GameState()
{
    delete this->level;
}

void GameState::initGame()
{
    if(this->level) delete this->level;
    ftl::LoadGame(*this->player);
    this->initLevel();
    this->changePotionShape();
}

void GameState::initLevel()
{
    this->level = new Level(player, player->getLevel(), atoi(player->getDoor().c_str()));
    this->level->entityUpdate();
    this->level->forceInterpolationUpdate();
}

void GameState::changeLevel()
{
    // Get destination data
    LevelExit* exit = this->level->getActiveExit();
    std::string mapFile = exit->getDestination();
    int entranceIndex = exit->getEntranceIndex();

    // Save game data
    this->player->setLevel(mapFile);
    this->player->setDoor(entranceIndex);
    this->player->stopSpeed();
    ftl::SaveGame(*this->player);
    this->level->saveLevelData();

    // Reset level
    delete this->level;
    this->initLevel();
}

void GameState::transitionUpdate()
{
    this->f_transitionCounter += Engine::getInstance()->getDelta();

    float progress = this->f_transitionCounter / this->f_transitionTime;
    if(progress > 1) progress = 1;
    if(!b_isLeaving) progress = 1 - progress;
    this->fadeOutPanel.setFillColor(sf::Color(0,0,0,progress*255));

    if(this->f_transitionCounter >= this->f_transitionTime)
    {
        // Reset the counter
        this->f_transitionCounter = 0.0f;

        // Player is leaving the level
        if(this->b_isLeaving)
        {
            this->changeLevel();
            this->b_isLeaving = false;
        }
        // Player is entering the level
        else
        {
            this->b_isTransitioning = false;
        }
    }
}

void GameState::update()
{
    if(this->b_reInit)
    {
        this->b_reInit = false;
        this->initGame();
    }

    // Update the level transition
    if(this->b_isTransitioning)
    {
        this->transitionUpdate();
    }
    // Update the game
    else
    {
        this->level->update();
        this->changePotionShape(); 
        this->changeDamageEffects();

        // Check level change
        if(this->level->didPlayerLeave())
        {
            this->b_isLeaving = true;
            this->b_isTransitioning = true;
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
            //std::cerr<<"Y ahora vuelvo a cargar\n";
        }
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
            Engine::getInstance()->renderDrawable(&this->fadeOutPanel);
        }
    }
}
