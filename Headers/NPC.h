#ifndef NPC_H
#define NPC_H

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>

#include "Engine.h"
#include "ResourceManager.h"

class NPC {

private:

    sf::RectangleShape shape;

    std::map<std::string, std::vector<std::string>> quotes;
    std::string currentQuoteSet;

    bool b_interactable;

    bool b_isShop;
    bool b_isFinalDoor;
    int i_isKey;
public:
    NPC(std::string sheetFile);
    ~NPC();

    // Returns the NPC's position
    sf::Vector2f getPosition();
    // Sets the NPC's position
    void setPosition(sf::Vector2f pos);
    // Returns the NPC's size
    sf::Vector2f getSize();
    // Sets the NPC's size
    void setSize(sf::Vector2f size);

    // Returns true if this NPC can be interacted with
    bool isInteractable();
    // Sets if this NPC can be interacted with
    void setInteractable(bool interactable);

    // Returns the lines this NPC says if is interacted with
    std::vector<std::string> getDialogue();
    // Returns true if this NPC triggers the shop state upon interaction
    bool getImShop();
    // Returns true if this NPC tis the final Boss's room door. If true, the Level will
    // make this not interactable if needed, and interacting with it will take to the final boss's room
    bool getImFinalDoor();
    // Returns true if this NPC triggers the "Get Boss Key" action upon interaction
    // This action is: player returns to Lobby and maybe an animation plays
    bool getImKey();
    // Returns the key type if it is a key. Otherwise returns -1
    int getKeyType();

    // Changes the texture / Sprite of this NPC
    void setNewTexture(std::string filename);
    // Changes the opacity of this npc
    void setOpacity(float newAlpha);

    void render();
};

#endif