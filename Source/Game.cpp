#include "Game.h"

Game::Game()
{
    this->state = new GameState();
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
        this->updateDelta();
        this->updateSFMLEvents();
        this->update();
        this->render();
    }
}
