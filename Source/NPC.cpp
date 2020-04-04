#include "NPC.h"
NPC::NPC(std::string sheetFile)
{
    FILE* file = fopen(("npcSheets/"+sheetFile+".json").c_str(), "rb");

    char buffer[65536];
    rapidjson::FileReadStream is(file, buffer, sizeof(buffer));
    rapidjson::Document document;
    document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);

    std::string textureFile = document["textureFile"].GetString();
    this->b_isShop = document["isShop"].GetBool();

    for(unsigned int i = 0; i < document["quotes"].Size() ; i++)
    {
        rapidjson::Value& state = document["quotes"][i];
        std::string stateName = state["state"].GetString();

        for(unsigned int j = 0; j < state["quotes"].Size() ; j++)
        {
            this->quotes[stateName].push_back(
                state["quotes"][j]["text"].GetString()
            );
        }
    }

    fclose(file);
    this->dialogo.setString(this->quotes["greeting"][0]);
    this->shape.setTexture(ResourceManager::getInstance()->loadTexture("resources/"+textureFile));
}

NPC::~NPC()
{

}

sf::Vector2f NPC::getPosition()
{
    return shape.getPosition();
}

void NPC::setPosition(sf::Vector2f pos)
{
    this->shape.setPosition(pos);
}

sf::Vector2f NPC::getSize()
{
    return this->shape.getSize();
}

void NPC::setSize(sf::Vector2f size)
{
    this->shape.setSize(size);
    shape.setOrigin(shape.getSize().x/2.0f, shape.getSize().y/2);
}

sf::Text NPC:: getDialogue()
{
    return dialogo;
}


bool NPC:: getImShop()
{
    return this->b_isShop;
}

void NPC:: render(){
    Engine::getInstance()->renderDrawable(&shape);
}
