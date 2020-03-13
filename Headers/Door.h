#ifndef DOOR_H
#define DOOR_H

#include "Engine.h"

class Door {
private:
    bool b_isOpen;
    sf::Vector2f vf_position;
    sf::RectangleShape shape;
public:
    Door();
    ~Door();

    void toggleOpenState();

    void update();
    void render();
};

#endif
