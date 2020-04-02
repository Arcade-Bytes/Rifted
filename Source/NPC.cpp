#include "NPC.h"
NPC::NPC(std::string textureName, float x, float y,bool hasShop, std::string dialogText)
{
    this->texture.loadFromFile("resources/"+textureName);
    this->sprite.setTexture(texture);

    //sprite->setOrigin(sprite->getLocalBounds().width/2.0f, sprite->getLocalBounds().height);
    this->sprite.setPosition(x,y);

    dialogo.setString(dialogText);
    //Si true es el NPC de la tienda, lo uso en Game para saber si meto estado Dialogo o Tienda
    ImShop = hasShop;
}

NPC::~NPC()
{

}

void NPC:: render(){

    Engine* engine = Engine::getInstance();

    engine->renderDrawable(&sprite);

}

sf::Vector2f NPC:: getPosition(){

    return sprite.getPosition();

}

sf::Text NPC:: getDialogue(){

    return dialogo;

}


bool NPC:: getImShop()
{
    return ImShop;
}

void NPC:: update()
{

}
