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
    //Position related
    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f pos);
    //Size related
    sf::Vector2f getSize();
    void setSize(sf::Vector2f size);
    //Checks if able to pick
    bool isWithinReach(sf::Vector2f pos);
    //Name related
    std::string getName();
    //Getters and setters
    void setIsPicked(bool picked);
    bool getIsPicked();

    void render();
};

#endif