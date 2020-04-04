#ifndef DOOR_H
#define DOOR_H

#include "Engine.h"
#include "Hitbox.h"
#include "ResourceManager.h"

class Door {
private:
    bool b_isOpen;
    sf::Vector2f vf_position;
    sf::RectangleShape shape;
    Hitbox* hitbox;

    std::string s_vinculationId;
public:
    Door();
    ~Door();

    // Accesors
    sf::Vector2f getPosition();
    void setPosition(const float&x, const float& y);
    void setPosition(sf::Vector2f pos);
    void setSize(sf::Vector2f size);

    void setVinculationId(std::string id);
    std::string getVinculationId();

    void toggleOpenState();

    void update();
    void render();
};

#endif
