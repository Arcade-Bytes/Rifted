#ifndef BREAKABLEDOOR_H
#define BREAKABLEDOOR_H

#include "Door.h"

class BreakableDoor : public Door {
private:

public:
    BreakableDoor();
    ~BreakableDoor();

    void update();
    void render();
};

#endif
