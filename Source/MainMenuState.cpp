#include "MainMenuState.h"


MainMenuState::MainMenuState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = MAINMENU_STATE;
    if (!tex.loadFromFile("resources/TheRift.jpg")) {
        std::cerr << "Error cargando la imagen TheRifted.png";
    exit(0);
    }

    if (!fuente.loadFromFile("resources/XOX.ttf")) {
        std::cerr << "Error cargando la fuente XOX.png";
    exit(0);
    }
    
    clock = new sf::Clock();

    fondoIni = new sf::Sprite(tex);

    fondoIni->setScale(2,2);

    fondoIni->setTextureRect(sf::IntRect(0,0,tex.getSize().x,tex.getSize().y) );

    texto = new sf::Text();

    texto->setFont(fuente);

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
    if(Engine::getInstance()->getKeyPressed(sf::Keyboard::Return)){
        this->changeState(MENU_STATE);
    }
}

void MainMenuState:: render(){

    Engine* engine = Engine::getInstance();
    engine->setFollowView(false);

    engine->renderDrawable(fondoIni);

    //TITLE
    texto->setString("Rifted");
    texto->setCharacterSize(216);
    texto->setColor(sf::Color::White);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    texto->setPosition(engine->getWindowSize().x/2,engine->getWindowSize().y/3);

    engine->renderDrawable(texto);


    //ENTER
    texto->setString("Press Enter to Continue");
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    texto->setPosition(engine->getWindowSize().x/2,engine->getWindowSize().y/4*3 );

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