#ifndef NPC_H
#define NPC_H

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>

#include "Engine.h"

class NPC {

private:
    sf::Text dialogo;

    sf::Texture texture;
    sf::RectangleShape shape;

    std::map<std::string, std::vector<std::string>> quotes;

    bool b_isShop;
public:

    NPC(std::string sheetFile);
    ~NPC();

    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getSize();
    void setSize(sf::Vector2f size);


    sf::Text getDialogue();
    bool getImShop();

    void render();
};

#endif