#include "TextState.h"


TextState::TextState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = TEXT_STATE;

    Engine* engine = Engine::getInstance();
    sf::Vector2f baseRes = sf::Vector2f(engine->getBaseResolution().x,engine->getBaseResolution().y);

    text_box = new sf::RectangleShape();
    text_box->setTexture(ResourceManager::getInstance()->loadTexture("resources/text_box.png"));
    text_box->setSize(sf::Vector2f(baseRes.x*0.8125f, baseRes.y*0.27777777f)); //1560 x 300 sobre 1920 x 1080
    text_box->setOrigin(baseRes.x*0.8125f/2.0f, 0);
    text_box->setPosition(baseRes.x/2.0f,baseRes.y*0.02f);

    dialogue.setColor(sf::Color::White);
    dialogue.setFont(*ResourceManager::getInstance()->loadFont("resources/XOX.ttf"));
    dialogue.setCharacterSize(baseRes.x * 0.028125f);
    dialogue.setPosition(baseRes.x * 0.15f,baseRes.y*0.1f);
}

TextState::~TextState(){
    delete text_box;
}

void TextState::initConversation()
{
    this->quotes = this->player->getNear();
    currentQuote = -1;
    this->nextQuote();
}

void TextState::setText(std::string text)
{
    dialogue.setString(text);
}

bool TextState::nextQuote()
{
    currentQuote++;
    if(currentQuote < (int)quotes.size())
    {
        dialogue.setString(quotes[currentQuote]);
        return true;
    }
    else
    {
        dialogue.setString("");
        return false;
    }
}

void TextState:: update()
{ 
    if(this->b_reInit)
    {
        this->b_reInit = false;
        this->initConversation();
    }

    if(Engine::getInstance()->getKeyPressed(sf::Keyboard::Return)){
        bool hasNextQuote = this->nextQuote();
        if(!hasNextQuote) this->changeState(GAME_STATE, false);
    }
}

void TextState:: render(float frameProgress)
{
    Engine* engine = Engine::getInstance();
    engine->setFollowView(false);

    engine->renderDrawable(text_box);
    engine->renderDrawable(&dialogue);
}
