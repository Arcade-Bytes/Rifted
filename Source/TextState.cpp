#include "TextState.h"


TextState::TextState(std::stack<State*>* states, Player* player, sf::Text dialogo)
    :State(states, player)
{
    this->Iam = TEXT_STATE;

    Engine* engine = Engine::getInstance();

    dialogue = dialogo;

    if (!fuente.loadFromFile("resources/XOX.ttf")) {
        std::cerr << "Error cargando la fuente XOX.png";
    exit(0);
    }

    //Cargamos la caja de texto
    if (!box.loadFromFile("resources/text_box.png")) {
            std::cerr << "Error cargando la imagen text_box.png";
        exit(0);
    }
    text_box = new sf::Sprite(box);
    text_box->setOrigin(text_box->getLocalBounds().width/2.0f, text_box->getLocalBounds().height/2.0f);
    text_box->setPosition(engine->getWindowSize().x/2,engine->getWindowSize().y/7);
    text_box->setScale(3,1.5);

    dialogue.setColor(sf::Color::White);
    dialogue.setOrigin(dialogue.getLocalBounds().width/2.0f, dialogue.getLocalBounds().height/2.0f);
    dialogue.setFont(fuente);
    dialogue.setCharacterSize(54);
    dialogue.setPosition(text_box->getOrigin().x,text_box->getOrigin().y);

}

TextState::~TextState(){
    delete text_box;
}

void TextState:: update()
{ 
    setText(player->getNear());
    if(Engine::getInstance()->getKeyPressed(sf::Keyboard::Return)){
        this->changeState(GAME_STATE);
    }
}

void TextState:: render(){

    Engine* engine = Engine::getInstance();
    engine->setFollowView(false);

    engine->renderDrawable(text_box);
    engine->renderDrawable(&dialogue);

}

void TextState::setText(std::string text){

    dialogue.setString(text);

}
