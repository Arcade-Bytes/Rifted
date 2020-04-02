#include "Game.h"

Game::Game()
{
    this->state = new GameState();
    this->updateStartTime = 0.0f;
}

Game::~Game()
{
    delete this->state;
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
    this->state->update();
}

void Game::render()
{
    Engine* engine = Engine::getInstance();
    engine->windowClear();

	this->state->render();

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
        this->render();
    }
}
