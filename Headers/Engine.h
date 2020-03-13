#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include "../include/config.h"

class Engine {
private:
    static Engine* instance;

    Engine();
    ~Engine();

    // SFML Properties
    sf::RenderWindow* window;
    sf::Clock clock;

    float delta;
    
public:
    Engine(const Engine&) = delete;
    Engine &operator= (const Engine&) = delete;

    // Accessors
    bool isWindowOpen();
    const float& getDelta();

    // Updates
    void updateDelta();
    void updateSFMLEvents();
    void updateInput();

    // Renders
    void RenderSprite(sf::Sprite* sprite);
    void RenderShape(sf::Shape* shape);
    void WindowClear();
    void WindowDisplay();

    static Engine* getInstance();
};

#endif
