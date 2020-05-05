#include "MainMenuState.h"


MainMenuState::MainMenuState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = MAINMENU_STATE;
    
    clock = new sf::Clock();

    Engine* engine = Engine::getInstance();
    ResourceManager* resource = ResourceManager::getInstance();
    sf::Vector2f baseResolution = sf::Vector2f(engine->getBaseResolution().x,engine->getBaseResolution().y);

    this->fondoIni = new sf::RectangleShape();
    this->fondoIni->setTexture(resource->loadTexture("resources/TheRift.jpg"));
    this->fondoIni->setSize(baseResolution);

    textoTitle = new sf::Text();
    textoTitle->setFont(*resource->loadFont("resources/XOX.ttf"));
    textoTitle->setString("Rifted");
    textoTitle->setCharacterSize(engine->getBaseResolution().x*0.1125f);
    textoTitle->setColor(sf::Color::White);
    textoTitle->setOrigin(textoTitle->getLocalBounds().width/2.0f,textoTitle->getLocalBounds().height/2.0f);
    textoTitle->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/3);

    textoPressEnter = new sf::Text();
    textoPressEnter->setFont(*resource->loadFont("resources/XOX.ttf"));
    textoPressEnter->setString("Press Enter to Continue");
    textoPressEnter->setCharacterSize(engine->getBaseResolution().x*0.028125f);
    textoPressEnter->setColor(sf::Color(255,255,255,0));
    textoPressEnter->setOrigin(textoPressEnter->getLocalBounds().width/2.0f,textoPressEnter->getLocalBounds().height/2.0f);
    textoPressEnter->setPosition(engine->getBaseResolution().x/2,engine->getBaseResolution().y/4*3 );

    alpha = prevAlpha = 0;
    dir = true;

}

MainMenuState::~MainMenuState()
{
    delete fondoIni;
    delete textoTitle;
    delete textoPressEnter;
    delete clock;
}

void MainMenuState:: update()
{
    ResourceManager::getInstance()->playMainMenu();
    
    Engine* engine = Engine::getInstance();
    if(engine->getKeyPressed(sf::Keyboard::Return)){
        this->changeState(MENU_STATE, false);
    }
    else if(engine->getKeyPressed(sf::Keyboard::Escape))
    {
        engine->windowClose();
    }

    // Alpha update
    if(clock->getElapsedTime().asMilliseconds()>(0.004f)){  //0.00004
        int steps = clock->getElapsedTime().asSeconds() / 0.004f;
        prevAlpha = alpha;
        if(dir==true)
            alpha+= steps;   
        else
            alpha-=steps;
        if(alpha > 255) alpha = 255;
        if(alpha < 0) alpha = 0;

        clock->restart();

        if(alpha>=255)
            dir=false;
        else if(alpha<=0)
            dir = true;
    }
}

void MainMenuState:: render(float frameProgress)
{
    Engine* engine = Engine::getInstance();
    engine->setFollowView(false);

    engine->renderDrawable(fondoIni);
    engine->renderDrawable(textoTitle);
    engine->renderDrawable(textoPressEnter);

    int finalAlpha = this->prevAlpha + (this->alpha - this->prevAlpha) * frameProgress;
    textoPressEnter->setColor(sf::Color(255,255,255,finalAlpha));
}

void MainMenuState:: endState(){

}