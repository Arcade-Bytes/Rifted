#include "Engine.h"

Engine* Engine::instance = 0;

Engine::Engine()
{
    this->window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Pengo Alpha ver.");
    delta = 0.0f;
}

Engine::~Engine()
{
    delete this->window;
}

// Accessors
bool Engine::isWindowOpen()
{
    return this->window->isOpen();
}

const float& Engine::getDelta()
{
    return this->delta;
}

// Updates
void Engine::updateDelta()
{
    this->delta = clock.restart().asSeconds();
}

void Engine::updateSFMLEvents()
{
    sf::Event event;
    while (window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window->close();
                break;

            default: break;
        }
    }
}

void Engine::updateInput()
{
    
}

// Renders
void Engine::RenderSprite(sf::Sprite* sprite)
{
    this->window->draw(*sprite);
}

void Engine::RenderShape(sf::Shape* shape)
{
    this->window->draw(*shape);
}

void Engine::WindowClear()
{
    this->window->clear();
}

void Engine::WindowDisplay()
{
    this->window->display();
}

Engine* Engine::getInstance()
{
    if(!instance) instance = new Engine;
    return instance;
}
