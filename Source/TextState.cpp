#include "TextState.h"


TextState::TextState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = TEXT_STATE;

    Engine* engine = Engine::getInstance();

    text_box = new sf::Sprite(*ResourceManager::getInstance()->loadTexture("resources/text_box.png"));
    text_box->setOrigin(text_box->getLocalBounds().width/2.0f, text_box->getLocalBounds().height/2.0f);
    text_box->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/7);
    text_box->setScale(3,1.5);

    dialogue.setColor(sf::Color::White);
    dialogue.setOrigin(dialogue.getLocalBounds().width/2.0f, dialogue.getLocalBounds().height/2.0f);
    dialogue.setFont(*ResourceManager::getInstance()->loadFont("resources/XOX.ttf"));
    dialogue.setCharacterSize(54);
    dialogue.setPosition(text_box->getOrigin().x,text_box->getOrigin().y);
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
