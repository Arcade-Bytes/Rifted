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

    bool b_isShop;
    int i_isKey;
public:

    NPC(std::string sheetFile);
    ~NPC();

    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getSize();
    void setSize(sf::Vector2f size);


    std::vector<std::string> getDialogue();
    bool getImShop();
    bool getImKey();
    int getKeyType();

    void render();
};

#endif