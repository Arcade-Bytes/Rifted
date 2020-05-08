#ifndef DOOR_H
#define DOOR_H

#include "Engine.h"
#include "Hitbox.h"
#include "ResourceManager.h"

class Door {
private:
    bool b_isOpen;
    bool b_isHorizontal;
    sf::Vector2f vf_position;
    sf::RectangleShape shape;
    Hitbox* hitbox;

    sf::IntRect doorRects[2];

    std::string s_vinculationId;
public:
    Door(bool isHorizontal = false);
    ~Door();

    // Returns the position of the door
    sf::Vector2f getPosition();
    // Sets the position of the door (with two floats)
    void setPosition(const float&x, const float& y);
    // Sets the position of the door (with an sf::Vector2f)
    void setPosition(sf::Vector2f pos);
    // Sets the size of the door (with an sf::Vector2f)
    void setSize(sf::Vector2f size);

    // Sets the vinculation name of the door. This name will be used by levers with the same name, later, to vinculate to this door and interact with it
    void setVinculationId(std::string id);
    // Returns the vinculation name of the door. This name is used by levers with the same name to vinculate to this door and interact with it
    std::string getVinculationId();

    // Toggles the door state (open/closed). This is used by vinculated levers when they are interacted with
    void toggleOpenState();

    void update();
    void render();
};

#endif
