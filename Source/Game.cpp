#include "Game.h"

Game::Game()
{
    this->player = new Player(100.0f);

    this->states.push(new MenuState(&states, player));
    this->states.push(new PauseState(&states, player));
    this->states.push(new ShopState(&states, player));
    this->states.push(new TextState(&states, player));
    this->states.push(new GameState(&states, player));
    this->states.push(new MainMenuState(&states, player));

    this->updateStartTime = 0.0f;
}

Game::~Game()
{
    delete this->player;
    while(!this->states.empty())
        this->states.pop();
}

void Game::updateDelta()
{
    Engine::getInstance()->updateDelta();
}

void Game::updateSFMLEvents()
{
    Engine::getInstance()->updateSFMLEvents();
}

void Game::update()
{
    if(!this->states.empty())
        this->states.top()->update();
}

void Game::render(float frameProgress)
{
    Engine* engine = Engine::getInstance();
    engine->windowClear();

    StateType type = states.top()->getIam();
    if(states.size() >= 2 && (type == PAUSE_STATE || type == TEXT_STATE))
    {
        State* topState = this->states.top();
        this->states.pop();
        this->states.top()->render(frameProgress);
        this->states.push(topState);
        this->states.top()->render(frameProgress);
    }
    else
    {
        if(!this->states.empty())
            this->states.top()->render(frameProgress);
    }

    engine->windowDisplay();
}

void Game::run()
{
    Engine* engine = Engine::getInstance();
    while(engine->isWindowOpen())
    {
        if((engine->getUpdateTime() - updateStartTime) > UPDATE_TIME)
        {
		    this->updateDelta();
            this->updateSFMLEvents();
            this->update();
            updateStartTime = engine->getUpdateTime();
        }
        float frameProgress = (engine->getUpdateTime() - updateStartTime) / (UPDATE_TIME);
        if(frameProgress > 1.0f) frameProgress = 1.0f;
        this->render(frameProgress);
    }
}
