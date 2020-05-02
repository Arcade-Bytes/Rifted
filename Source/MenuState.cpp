#include "MenuState.h"

MenuState::MenuState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = MENU_STATE;

    clock = new sf::Clock();

    Engine* engine = Engine::getInstance();
    ResourceManager* resources = ResourceManager::getInstance();
    sf::Vector2f baseResolution = sf::Vector2f(engine->getBaseResolution().x,engine->getBaseResolution().y);

    this->fondoIni = new sf::RectangleShape();
    this->fondoIni->setTexture(resources->loadTexture("resources/TheRift.jpg"));
    this->fondoIni->setSize(baseResolution);

    flecha_selectorR = new sf::Sprite(*resources->loadTexture("resources/pixel-arrow-png-18.png"));
    flecha_selectorR->setScale(0.4,0.4);
    flecha_selectorR->setOrigin(flecha_selectorR->getLocalBounds().width/2.0f, flecha_selectorR->getLocalBounds().height/2.0f);

    flecha_selectorL = new sf::Sprite(*resources->loadTexture("resources/pixel-arrow-png-18.png"));
    flecha_selectorL->setScale(0.4,0.4);
    flecha_selectorL->setOrigin(flecha_selectorL->getLocalBounds().width/2.0f, flecha_selectorL->getLocalBounds().height/2.0f);
    flecha_selectorL->rotate(180);

    texto = new sf::Text();

    texto->setFont(*resources->loadFont("resources/XOX.ttf"));

    arrowMovement = prevArrowMovement = 0;

    dir = true;

    seleccion = 1;
}

MenuState::~MenuState(){
    delete fondoIni;
    delete texto;
    delete clock;
    delete flecha_selectorL;
    delete flecha_selectorR; 
}

void MenuState:: update(){
    Engine* engine = Engine::getInstance();
    if(engine->getKeyPressed(sf::Keyboard::Down)){
        if(seleccion == 4)
            seleccion = 1;
        else
            seleccion++;
    }
    if(engine->getKeyPressed(sf::Keyboard::Up)){
        if(seleccion == 1)
            seleccion = 4;
        else
            seleccion--;
    }

    if(engine->getKeyPressed(sf::Keyboard::Return))
    {
        switch(seleccion){
            case 1: //Opcion Partida Nueva
                ftl::ResetSaveFile();
                ftl::LoadGame(*this->player);
                this->changeState(GAME_STATE, true);
                break;   
            case 2: //Futuro Cargar Partida
                ftl::LoadGame(*this->player);
                this->changeState(GAME_STATE, true);
                break;
            case 3: break; //Futuro Ajustes
            case 4: engine->windowClose(); break;//Opcion Exit
        }
    }

    if(engine->getKeyPressed(sf::Keyboard::Escape))
    {
        this->changeState(MAINMENU_STATE, true);
    }

    if(clock->getElapsedTime().asMilliseconds()>(0.004f)){  //0.00004
        int steps = clock->getElapsedTime().asSeconds() / 0.004f;
        prevArrowMovement = arrowMovement;
        if(dir==true)
            arrowMovement+=steps;
        else 
            arrowMovement-=steps;

        if(arrowMovement>30)arrowMovement=30;
        if(arrowMovement<-30)arrowMovement=-30;
        
        clock->restart();

        if(arrowMovement>=30) dir=false; else if(arrowMovement<=-30) dir = true;
    }
}

void MenuState:: render(float frameProgress){

    Engine* engine = Engine::getInstance();
    engine->setFollowView(false);

    engine->renderDrawable(fondoIni);
    drawText();
    drawArrow(frameProgress);
}

void MenuState:: drawText(){

    Engine* engine = Engine::getInstance();

    engine->renderDrawable(fondoIni);

    texto->setString("Nueva Partida");
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/2);
    texto->setCharacterSize(engine->getBaseResolution().x*0.028125f);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Continuar Partida");
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/2 + engine->getBaseResolution().y*0.1);
    texto->setCharacterSize(engine->getBaseResolution().x*0.028125f);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Ajustes");
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/2 + engine->getBaseResolution().y*0.2);
    texto->setCharacterSize(engine->getBaseResolution().x*0.028125f);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Salir");
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/2 + engine->getBaseResolution().y*0.3);
    texto->setCharacterSize(engine->getBaseResolution().x*0.028125f);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    //TITLE
    texto->setString("Rifted");
    texto->setCharacterSize(engine->getBaseResolution().x*0.1125f);
    texto->setColor(sf::Color::White);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/4);

    engine->renderDrawable(texto);


}


void MenuState:: drawArrow(float frameProgress){

    Engine* engine = Engine::getInstance();

        switch(seleccion){
            case 1:
                flecha_selectorR->setPosition(engine->getBaseResolution().x/2 + 300 ,engine->getBaseResolution().y/2+ 10);
                flecha_selectorL->setPosition(engine->getBaseResolution().x/2 - 300 ,engine->getBaseResolution().y/2+ 10);
                break;

            case 2:
                flecha_selectorR->setPosition(engine->getBaseResolution().x/2 + 380 ,engine->getBaseResolution().y/2 + engine->getBaseResolution().y*0.1+10);
                flecha_selectorL->setPosition(engine->getBaseResolution().x/2 - 380 ,engine->getBaseResolution().y/2 + engine->getBaseResolution().y*0.1+10);
                break;

            case 3:
                flecha_selectorR->setPosition(engine->getBaseResolution().x/2 + 200 ,engine->getBaseResolution().y/2 + engine->getBaseResolution().y*0.2+10);
                flecha_selectorL->setPosition(engine->getBaseResolution().x/2 - 200 ,engine->getBaseResolution().y/2 + engine->getBaseResolution().y*0.2+10);
                break;

            case 4:
                flecha_selectorR->setPosition(engine->getBaseResolution().x/2 + 170 ,engine->getBaseResolution().y/2 + engine->getBaseResolution().y*0.3+10);
                flecha_selectorL->setPosition(engine->getBaseResolution().x/2 - 170 ,engine->getBaseResolution().y/2 + engine->getBaseResolution().y*0.3+10);
                break;
        }

        int finalMove = this->prevArrowMovement + (this->arrowMovement - this->prevArrowMovement) * frameProgress;
        flecha_selectorL->move(finalMove,0);
        flecha_selectorR->move(-finalMove,0);

        engine->renderDrawable(flecha_selectorR);
        engine->renderDrawable(flecha_selectorL);
}
