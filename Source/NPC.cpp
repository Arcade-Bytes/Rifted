#include "NPC.h"
NPC::NPC(std::string sheetFile)
{
    FILE* file = fopen(("npcSheets/"+sheetFile+".json").c_str(), "rb");

    char buffer[65536];
    rapidjson::FileReadStream is(file, buffer, sizeof(buffer));
    rapidjson::Document document;
    document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);

    std::string textureFile = document["textureFile"].GetString();

    if(document.HasMember("isShop"))
        this->b_isShop = document["isShop"].GetBool();
    else
        this->b_isShop = false;

    if(document.HasMember("isKey"))
        this->i_isKey = document["isKey"].GetInt();
    else
        this->i_isKey = -1;

    if(document.HasMember("isFinalBossDoor"))
        this->b_isFinalDoor = document["isFinalBossDoor"].GetBool();
    else
        this->b_isFinalDoor = false;

    currentQuoteSet = "";
    for(unsigned int i = 0; i < document["quotes"].Size() ; i++)
    {
        rapidjson::Value& state = document["quotes"][i];
        std::string stateName = state["state"].GetString();
        if(currentQuoteSet == "") currentQuoteSet = stateName;

        for(unsigned int j = 0; j < state["quotes"].Size() ; j++)
        {
            this->quotes[stateName].push_back(
                state["quotes"][j]["text"].GetString()
            );
        }
    }

    fclose(file);

    this->shape.setTexture(ResourceManager::getInstance()->loadTexture("resources/"+textureFile));
    this->b_interactable = true;
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

bool NPC::isInteractable()
{
    return this->b_interactable;
}

void NPC::setInteractable(bool interactable)
{
    this->b_interactable = interactable;
}

std::vector<std::string> NPC:: getDialogue()
{
    return this->quotes[currentQuoteSet];
}


bool NPC::getImShop()
{
    return this->b_isShop;
}

bool NPC::getImFinalDoor()
{
    return this->b_isFinalDoor;
}

bool NPC::getImKey()
{
    return this->i_isKey >= 0;
}

int NPC::getKeyType()
{
    return this->i_isKey;
}

void NPC::setNewTexture(std::string filename)
{
    this->shape.setTexture(ResourceManager::getInstance()->loadTexture("resources/"+filename));
}

void NPC::setOpacity(float newAlpha)
{
    this->shape.setFillColor(sf::Color(255,255,255,newAlpha*255));
}

void NPC::render(){
    Engine::getInstance()->renderDrawable(&shape);
}
