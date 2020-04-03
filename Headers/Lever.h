#ifndef LEVER_H
#define LEVER_H

#include "Door.h"

class Lever {
private:
    std::vector<Door*> v_doorPointers;
    bool b_isActive;
    sf::Vector2f vf_position;
    sf::RectangleShape shape;
public:
    Lever();
    ~Lever();

    // Accesors
    sf::Vector2f getPosition();
    void setPosition(const float&x, const float& y);
    void setPosition(sf::Vector2f pos);
    void setSize(sf::Vector2f size);

    void addDoor(Door* door);
    bool getIsActive();
    void interact();

    void update();
    void render();
};

#endif
