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

    flecha_selectorR = new sf::RectangleShape();
    flecha_selectorR->setTexture(resources->loadTexture("resources/pixel-arrow-png-18.png"));
    flecha_selectorR->setSize(sf::Vector2f(baseResolution.x*0.041666f,baseResolution.y*0.074f)); // 80x80 sobre 1920x1080
    flecha_selectorR->setOrigin(flecha_selectorR->getSize()/2.0f);

    flecha_selectorL = new sf::RectangleShape();
    flecha_selectorL->setTexture(resources->loadTexture("resources/pixel-arrow-png-18.png"));
    flecha_selectorL->rotate(180);
    flecha_selectorL->setSize(sf::Vector2f(baseResolution.x*0.041666f,baseResolution.y*0.074f)); // 80x80 sobre 1920x1080
    flecha_selectorL->setOrigin(flecha_selectorL->getSize()/2.0f);

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
            case 1: // Partida Nueva
                ftl::ResetSaveFile();
                ftl::LoadGame(*this->player);
                this->changeState(GAME_STATE, true);
                break;   
            case 2: // Cargar Partida
                ftl::LoadGame(*this->player);
                this->changeState(GAME_STATE, true);
                break;
            case 3: // Ver los créditos
                this->changeState(CREDITS_STATE, true);
                break;
            case 4: engine->windowClose(); break;//Opcion Exit
        }
    }

    if(engine->getKeyPressed(sf::Keyboard::Escape))
    {
        this->changeState(MAINMENU_STATE, true);
    }

    // Arrow movement
    if(clock->getElapsedTime().asMilliseconds()>(0.004f)){  //0.00004
        int steps = clock->getElapsedTime().asSeconds() / 0.004f;
        prevArrowMovement = arrowMovement;
        if(dir==true)
            arrowMovement+=(steps/30.0f);
        else 
            arrowMovement-=(steps/30.0f);

        if(arrowMovement>1.0f)arrowMovement=1.0f;
        if(arrowMovement<-1.0f)arrowMovement=-1.0f;
        
        clock->restart();

        if(arrowMovement>=1.0f) dir=false; else if(arrowMovement<=-1.0f) dir = true;
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

    texto->setCharacterSize(engine->getBaseResolution().x*0.028125f);

    texto->setString("Nueva Partida");
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/2);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Continuar Partida");
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/2 + engine->getBaseResolution().y*0.1);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Creditos");
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/2 + engine->getBaseResolution().y*0.2);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Salir");
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/2 + engine->getBaseResolution().y*0.3);
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
    sf::Vector2f baseRes = sf::Vector2f(engine->getBaseResolution().x,engine->getBaseResolution().y);

    float centerX = baseRes.x/2.0f;
    float centerY = baseRes.y/2.0f;
    switch(seleccion){
        case 1:
            flecha_selectorR->setPosition(centerX + baseRes.x*(220/1080.0f) ,centerY+ baseRes.y*(10/1080.0f));      // 1260, 550
            flecha_selectorL->setPosition(centerX - baseRes.x*(220/1080.0f) ,centerY+ baseRes.y*(10/1080.0f));      // 660, 550
            break;

        case 2:
            flecha_selectorR->setPosition(centerX + baseRes.x*(250/1080.0f) ,centerY + baseRes.y*(118/1080.0f));    // 1340, 118
            flecha_selectorL->setPosition(centerX - baseRes.x*(250/1080.0f) ,centerY + baseRes.y*(118/1080.0f));    // 580, 118
            break;

        case 3:
            flecha_selectorR->setPosition(centerX + baseRes.x*(160/1080.0f) ,centerY + baseRes.y*(226/1080.0f));    // 1160, 226
            flecha_selectorL->setPosition(centerX - baseRes.x*(160/1080.0f) ,centerY + baseRes.y*(226/1080.0f));    // 760, 226
            break;

        case 4:
            flecha_selectorR->setPosition(centerX + baseRes.x*(130/1080.0f) ,centerY + baseRes.y*(334/1080.0f));    // 1130, 334
            flecha_selectorL->setPosition(centerX - baseRes.x*(130/1080.0f) ,centerY + baseRes.y*(334/1080.0f));    // 790, 334
            break;
    }

    float finalMove = this->prevArrowMovement + (this->arrowMovement - this->prevArrowMovement) * frameProgress;
    finalMove *= engine->getBaseResolution().x * 0.015625f;
    flecha_selectorL->move(finalMove,0);
    flecha_selectorR->move(-finalMove,0);

    engine->renderDrawable(flecha_selectorR);
    engine->renderDrawable(flecha_selectorL);
}
