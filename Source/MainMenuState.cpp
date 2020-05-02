#include "MainMenuState.h"


MainMenuState::MainMenuState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = MAINMENU_STATE;
    
    clock = new sf::Clock();

    ResourceManager* resource = ResourceManager::getInstance();
    fondoIni = new sf::Sprite(*resource->loadTexture("resources/TheRift.jpg"));
    fondoIni->setScale(2,2);
    fondoIni->setTextureRect(sf::IntRect(0,0,
        resource->getTexture("resources/TheRift.jpg")->getSize().x,
        resource->getTexture("resources/TheRift.jpg")->getSize().y
    ));

    texto = new sf::Text();
    texto->setFont(*resource->loadFont("resources/XOX.ttf"));

    i = 0;
    dir = true;
}
MainMenuState::~MainMenuState(){
    delete fondoIni;
    delete texto;
    delete clock;
}

void MainMenuState:: update()
{
    Engine* engine = Engine::getInstance();
    if(engine->getKeyPressed(sf::Keyboard::Return)){
        this->changeState(MENU_STATE, false);
    }
    else if(engine->getKeyPressed(sf::Keyboard::Escape))
    {
        engine->windowClose();
    }
}

void MainMenuState:: render(float frameProgress){

    Engine* engine = Engine::getInstance();
    engine->setFollowView(false);

    engine->renderDrawable(fondoIni);

    //TITLE
    texto->setString("Rifted");
    texto->setCharacterSize(216);
    texto->setColor(sf::Color::White);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/3);

    engine->renderDrawable(texto);


    //ENTER
    texto->setString("Press Enter to Continue");
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/4*3 );

    if(clock->getElapsedTime().asSeconds()>(0.0004)){  //0.00004
        if(dir==true)
            i++;
            
        else i--;

        texto->setColor(sf::Color(255,255,255,i));
        clock->restart();

        if(i==255) dir=false; else if(i==0) dir = true;

    }

    engine->renderDrawable(texto);


}

void MainMenuState:: endState(){

}