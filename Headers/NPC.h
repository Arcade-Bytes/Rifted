#ifndef NPC_H
#define NPC_H

#include <SFML/Graphics.hpp>
#include "Engine.h"

class NPC {

    private:
        sf::Text dialogo;
        sf::Texture texture;
        sf::Sprite sprite;
        bool ImShop;

    public:

        NPC(std::string textureName, float x, float y,bool hasShop, std::string dialogText);
        ~NPC();

        void render();
        void update();
        sf::Vector2f getPosition();
        sf::Text getDialogue();
        bool getImShop();


};

#endif