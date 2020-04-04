#ifndef PICKABLE_H
#define PICKABLE_H

#include "Engine.h"
#include "ResourceManager.h"

class Pickable {

private:
    sf::RectangleShape shape;

    bool b_isPicked;
    std::string name;
public:
    Pickable(bool isPicked);
    Pickable(bool isPicked, std::string newName);
    ~Pickable();

    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getSize();
    void setSize(sf::Vector2f size);

    bool isWithinReach(sf::Vector2f pos);
    std::string getName();
    void setIsPicked(bool picked);
    bool getIsPicked();

    void render();
};

#endif