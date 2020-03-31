#include "Engine.h"

Engine* Engine::instance = 0;

Engine::Engine()
{
    this->window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Rifted Beta version");
    delta = 0.0f;

    this->view.setViewport(sf::FloatRect(0.0f,0.0f,1.0f,1.0f));
    this->view.setSize(this->window->getSize().x, this->window->getSize().y);
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

// Views
void Engine::setViewCenter(sf::Vector2f center)
{
    this->view.setCenter(center);
    this->window->setView(view);
}

// Input related
void Engine::resetPressedKeys()
{
    this->pressedKeys.clear();
}

bool Engine::getKeyPressed(sf::Keyboard::Key key)
{
    return this->pressedKeys[key];
}

// Updates
void Engine::updateDelta()
{
    this->delta = clock.restart().asSeconds();
}

void Engine::updateSFMLEvents()
{
    this->resetPressedKeys();

    //std::cout << "Update SFML" << std::endl;
    sf::Event event;
    while (window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed:
                this->pressedKeys[event.key.code] = true;

            default: break;
        }
    }
}

void Engine::updateInput()
{
    
}

// Renders
void Engine::renderDrawable(sf::Drawable* drawable)
{
    this->window->draw(*drawable);
}

void Engine::windowClear()
{
    this->window->clear();
}

void Engine::windowDisplay()
{
    this->window->display();
}

Engine* Engine::getInstance()
{
    if(!instance) instance = new Engine;
    return instance;
}
