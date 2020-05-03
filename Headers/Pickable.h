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
    Pickable(bool isPicked, std::string newName = "");
    ~Pickable();
    // Returns this pickable's position
    sf::Vector2f getPosition();
    // Sets this piackable's position
    void setPosition(sf::Vector2f pos);
    // Returns this pickable's size
    sf::Vector2f getSize();
    // Sets this piackable's size
    void setSize(sf::Vector2f size);
    // Checks if this pickable is within reach, so you can get it
    bool isWithinReach(sf::Vector2f pos);
    // Gets the pickable's assigned name
    std::string getName();
    // Set if this pickable is already picked
    void setIsPicked(bool picked);
    // Get if this pickable is already picked
    bool getIsPicked();

    void render();
};

#endif