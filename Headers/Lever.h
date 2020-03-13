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

    void addDoor(Door* door);
    void interact();

    void update();
    void render();
};

#endif
