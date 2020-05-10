#ifndef LEVELEXIT_H
#define LEVELEXIT_H

#include "Engine.h"
#include "Player.h"

class LevelExit {
private:
    Hitbox* hitbox;

    std::string s_destination;
    int i_entranceIndex;
public:
    LevelExit(std::string destination, const int& entranceIndex);
    ~LevelExit();

    // Accesors
    sf::Vector2f getPosition();
    void setPosition(const float&x, const float& y);
    void setPosition(sf::Vector2f pos);
    void setSize(sf::Vector2f size);

    /**
    *   Changing level stuff
    **/
    std::string getDestination();
    int getEntranceIndex();
    /**
     * Checks player collision with LevelExit
     **/
    bool checkPlayerCollision(Player* player);

    void render();
};

#endif
