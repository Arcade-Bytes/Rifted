#include "Game.h"

Game::Game()
{
    this->player = new Player(100.0f);

    this->states.push(new MenuState(&states, player));
    this->states.push(new PauseState(&states, player));
    this->states.push(new ShopState(&states, player));
    this->states.push(new TextState(&states, player, sf::Text()));
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

void Game::render()
{
    Engine* engine = Engine::getInstance();
    engine->windowClear();

    if(!this->states.empty())
        this->states.top()->render();

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
