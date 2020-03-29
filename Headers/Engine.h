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

    sf::View view;

    float delta;
public:
    Engine(const Engine&) = delete;
    Engine &operator= (const Engine&) = delete;

    // Accessors
    bool isWindowOpen();
    const float& getDelta();

    // Views
    void setViewCenter(sf::Vector2f center);

    // Updates
    void updateDelta();
    void updateSFMLEvents();
    void updateInput();

    // Renders
    void renderDrawable(sf::Drawable* drawable);
    void windowClear();
    void windowDisplay();

    static Engine* getInstance();
};

#endif
