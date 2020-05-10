#ifndef LEVER_H
#define LEVER_H

#include "ResourceManager.h"
#include "Door.h"

class Lever {
private:
    std::vector<Door*> v_doorPointers;
    bool b_isActive;
    sf::Vector2f vf_position;
    sf::RectangleShape shape;
    sf::Clock toggleClock;
public:
    Lever();
    ~Lever();

    // Accesors
    sf::Vector2f getPosition();
    void setPosition(const float&x, const float& y);
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getSize();
    void setSize(sf::Vector2f size);
    bool getIsActive();

    /**
     * Binding door to lever 
     **/
    void addDoor(Door* door);

    void update();
    void render();

    //Toggle control
    float getToggleTime();
    void interact(bool playSound = true);
    void restartToggleTime();
};

#endif
