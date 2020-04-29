#include "Engine.h"

Engine* Engine::instance = 0;

Engine::Engine()
{
    this->window = new sf::RenderWindow(sf::VideoMode(720, 360), "Rifted Beta version");
    baseResolution = {this->window->getSize().x,this->window->getSize().y};
    delta = 0.0f;

    this->view.setViewport(sf::FloatRect(0.0f,0.0f,1.0f,1.0f));
    this->view.setSize(this->window->getSize().x, this->window->getSize().y);
    this->uiView = this->window->getDefaultView();
    this->uiView.setViewport(sf::FloatRect(0.0f,0.0f,1.0f,1.0f));
    this->uiView.setSize(this->window->getSize().x, this->window->getSize().y);
}

Engine::~Engine()
{
    delete this->window;
}

// Accessors
void Engine::windowClose()
{
    this->window->close();
}

bool Engine::isWindowOpen()
{
    return this->window->isOpen();
}

sf::Vector2u Engine::getWindowSize()
{
    return this->window->getSize();
}

sf::Vector2u Engine::getBaseResolution()
{
    return this->baseResolution;
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


void Engine::resizeViews(const float& width, const float& height)
{
    float xProportion = baseResolution.x / width;
    float yProportion = baseResolution.y / height;

    if(xProportion < yProportion)
    {
        float newProportion = xProportion/yProportion;
        this->view.setViewport(sf::FloatRect(
            (1-newProportion)/2, 0.0f,
            1.0f * newProportion,
            1.0f
        ));
        this->uiView.setViewport(sf::FloatRect(
            (1-newProportion)/2, 0.0f,
            1.0f * newProportion,
            1.0f
        ));
    }
    else
    {
        float newProportion = yProportion/xProportion;
        this->view.setViewport(sf::FloatRect(
            0.0f,0.0f,
            1.0f,
            1.0f * newProportion
        ));
        this->uiView.setViewport(sf::FloatRect(
            0.0f,0.0f,
            1.0f,
            1.0f * newProportion
        ));
    }
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

    sf::Event event;
    while (window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed:
                this->pressedKeys[event.key.code] = true;
                break;
            case sf::Event::Resized:
                this->resizeViews(event.size.width, event.size.height);
                break;  

            default: break;
        }
    }
}

float Engine::getUpdateTime()
{
    return this->updateClock.getElapsedTime().asSeconds();
}

// Renders
void Engine::renderDrawable(sf::Drawable* drawable)
{
    this->window->draw(*drawable);
}

void Engine::setFollowView(bool follow)
{
    this->window->setView(follow ? view : uiView);
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
