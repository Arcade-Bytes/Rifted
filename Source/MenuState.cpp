#include "MenuState.h"

MenuState::MenuState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = MENU_STATE;

    clock = new sf::Clock();

    ResourceManager* resources = ResourceManager::getInstance();
    sf::Texture* riftTex = resources->loadTexture("resources/TheRift.jpg");
    fondoIni = new sf::Sprite(*riftTex);

    fondoIni->setScale(2,2);

    fondoIni->setTextureRect(sf::IntRect(0,0,riftTex->getSize().x,riftTex->getSize().y) );

    flecha_selectorR = new sf::Sprite(*resources->loadTexture("resources/pixel-arrow-png-18.png"));

    flecha_selectorR->setScale(0.4,0.4);

    flecha_selectorR->setOrigin(flecha_selectorR->getLocalBounds().width/2.0f, flecha_selectorR->getLocalBounds().height/2.0f);

    flecha_selectorL = new sf::Sprite(*resources->loadTexture("resources/pixel-arrow-png-18.png"));

    flecha_selectorL->setScale(0.4,0.4);

    flecha_selectorL->setOrigin(flecha_selectorL->getLocalBounds().width/2.0f, flecha_selectorL->getLocalBounds().height/2.0f);

    flecha_selectorL->rotate(180);

    texto = new sf::Text();

    texto->setFont(*resources->loadFont("resources/XOX.ttf"));

    i = 0;

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
}

void MenuState:: render(float frameProgress){

    Engine* engine = Engine::getInstance();
    engine->setFollowView(false);

    engine->renderDrawable(fondoIni);
    drawText();
    drawArrow();

}

void MenuState:: drawText(){

    Engine* engine = Engine::getInstance();

    engine->renderDrawable(fondoIni);

    texto->setString("Nueva Partida");
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/2);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Continuar Partida");
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/2 + 100);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Ajustes");
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/2 + 200);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Salir");
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/2 + 300);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    //TITLE
    texto->setString("Rifted");
    texto->setCharacterSize(216);
    texto->setColor(sf::Color::White);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    texto->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/4);

    engine->renderDrawable(texto);


}


void MenuState:: drawArrow(){

    Engine* engine = Engine::getInstance();

        switch(seleccion){
            case 1:

                flecha_selectorR->setPosition(engine->getBaseResolution().x/2 + 300 ,engine->getBaseResolution().y/2+ 10);
                

                flecha_selectorL->setPosition(engine->getBaseResolution().x/2 - 300 ,engine->getBaseResolution().y/2+ 10);
                     
            
            break;

            case 2:

                flecha_selectorR->setPosition(engine->getBaseResolution().x/2 + 380 ,engine->getBaseResolution().y/2+ 110);
                

                flecha_selectorL->setPosition(engine->getBaseResolution().x/2 - 380 ,engine->getBaseResolution().y/2+ 110);
                
            
            break;

            case 3:

                flecha_selectorR->setPosition(engine->getBaseResolution().x/2 + 200 ,engine->getBaseResolution().y/2+ 210);
                

                flecha_selectorL->setPosition(engine->getBaseResolution().x/2 - 200 ,engine->getBaseResolution().y/2+ 210);
                

            
            break;

            case 4:

                flecha_selectorR->setPosition(engine->getBaseResolution().x/2 + 170 ,engine->getBaseResolution().y/2+ 310);
                

                flecha_selectorL->setPosition(engine->getBaseResolution().x/2 - 170 ,engine->getBaseResolution().y/2+ 310);
                
            
            break;
        }

       if(clock->getElapsedTime().asSeconds()>(0.00004)){

            if(dir==true)
                i++;
            
            else i--;

            flecha_selectorL->move(i,0);
            flecha_selectorR->move(-i,0);
            
            clock->restart();

            if(i==30) dir=false; else if(i==-30) dir = true;

        }

        engine->renderDrawable(flecha_selectorR);
        engine->renderDrawable(flecha_selectorL);

        

}

void MenuState:: endState(){

}