#include "PauseState.h"

#define maxLvImprovement 3  //Nivel Maximo de mejora

PauseState::PauseState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = PAUSE_STATE;


    //Establecemos donde empieza la flecha
    seleccion = 1;
    //Inicializamos el reloj
    clock = new sf::Clock();
    //Paramentros para la animacion de la flecha
    dir = true;
    arrowMove = prevArrowMove = 0;

    loadAssets();

    //Rellenamos variables
    this->initPlayerData();
}

PauseState::~PauseState()
{
    delete texto;
    delete filtro;
    delete flecha_selector;
    delete heart_upgrade;
    delete sword_upgrade;
    delete coin_purse;
    delete kill_count;
    delete shield_upgrade;
    delete hammer_upgrade;
    delete bow_upgrade;
    delete parchment;
    delete mineKey;
    delete cementeryKey;
    delete towerKey;
    delete clock;
}

void PauseState::loadAssets()
{
    Engine* engine = Engine::getInstance();
    ResourceManager* resources = ResourceManager::getInstance();
    sf::Vector2f baseRes = {(float)engine->getBaseResolution().x,(float)engine->getBaseResolution().y};

    //Cargamos el filtro
    filtro = new sf::RectangleShape();
    filtro->setFillColor(sf::Color(0,0,0,120));
    filtro->setSize(sf::Vector2f(engine->getBaseResolution().x,engine->getBaseResolution().y));

    //Cargamos la fuente de texto
    texto= new sf::Text();
    texto->setFont(*resources->loadFont("resources/XOX.ttf"));
    texto->setColor(sf::Color::White);

    //Cargamos la flecha
    flecha_selector = new sf::RectangleShape();
    flecha_selector->setTexture(resources->loadTexture("resources/pixel-arrow-png-18.png"));
    flecha_selector->rotate(180);
    flecha_selector->setSize(sf::Vector2f(baseRes.x*0.041666f,baseRes.y*0.074f)); // 80x80 sobre 1920x1080
    flecha_selector->setOrigin(flecha_selector->getSize()/2.0f);

    //Cargamos el pergamino
    parchment = new sf::RectangleShape();
    parchment->setTexture(resources->loadTexture("resources/parchment.png"));
    parchment->setPosition(baseRes.x*1.08f,-baseRes.y*0.4);
    parchment->setSize(sf::Vector2f(baseRes.x*1.0f,baseRes.y*1.3f));
    parchment->rotate(90);

    //Cargamos las mejoras de vida
    heart_upgrade = new sf::RectangleShape();
    heart_upgrade->setTexture(resources->loadTexture("resources/health_upgrade.png"));
    heart_upgrade->setSize(sf::Vector2f(baseRes.x*0.0625f,baseRes.y*0.1111111f)); // 120x120 sobre 1920x1080
    heart_upgrade->setOrigin(heart_upgrade->getSize()/2.0f);
    heart_upgrade->setPosition(baseRes.x/2*1.15,baseRes.y/5 + baseRes.y*(410.0f/1080.0f));
    //heart_upgrade->setScale(0.1,0.1);

    //Cargamos las mejoras de espada
    sword_upgrade = new sf::RectangleShape();
    sword_upgrade->setTexture(resources->loadTexture("resources/sword_upgrade.png"));
    sword_upgrade->setSize(sf::Vector2f(baseRes.x*0.0469f,baseRes.y*0.08333f)); // 90x90 sobre 1920x1080
    sword_upgrade->setOrigin(sword_upgrade->getSize()/2.0f);
    sword_upgrade->setPosition(baseRes.x/2*1.4,baseRes.y/5 + baseRes.y*(400.0f/1080.0f));
    //sword_upgrade->setScale(0.13,0.13);

    //Cargamos las monedas
    coin_purse = new sf::RectangleShape();
    coin_purse->setTexture(resources->loadTexture("resources/coin.png"));
    coin_purse->setSize(sf::Vector2f(baseRes.x*0.03125f,baseRes.y*0.05555555f)); // 60x60 sobre 1920x1080
    coin_purse->setOrigin(sword_upgrade->getSize()/2.0f);
    coin_purse->setPosition(baseRes.x/2*1.12,baseRes.y/5 + baseRes.y*(120.0f/1080.0f));
    //coin_purse->setScale(0.1,0.1);

    //Cargamos las kills
    kill_count = new sf::RectangleShape();
    kill_count->setTexture(resources->loadTexture("resources/skull.png"));
    kill_count->setSize(sf::Vector2f(baseRes.x*0.03125f,baseRes.y*0.05555555f)); // 60x60 sobre 1920x1080
    kill_count->setOrigin(sword_upgrade->getSize()/2.0f);
    kill_count->setPosition(baseRes.x/2*1.12,baseRes.y/5 + baseRes.y*(220.0f/1080.0f));
    //kill_count->setScale(0.1,0.1);

    //Cargamos el escudo
    shield_upgrade = new sf::RectangleShape();
    shield_upgrade->setTexture(resources->loadTexture("resources/shield.png"));
    shield_upgrade->setSize(sf::Vector2f(baseRes.x*0.0469f,baseRes.y*0.08333f)); // 90x90 sobre 1920x1080
    shield_upgrade->setOrigin(shield_upgrade->getSize()/2.0f);
    shield_upgrade->setPosition(baseRes.x/2*1.65,baseRes.y/5 + baseRes.y*(400.0f/1080.0f));
    //shield_upgrade->setScale(0.3,0.3);

    //Cargamos el martillo
    hammer_upgrade = new sf::RectangleShape();
    hammer_upgrade->setTexture(resources->loadTexture("resources/hammer.png"));
    hammer_upgrade->setSize(sf::Vector2f(baseRes.x*0.0469f,baseRes.y*0.08333f)); // 90x90 sobre 1920x1080
    hammer_upgrade->setOrigin(hammer_upgrade->getSize()/2.0f);
    hammer_upgrade->setPosition(baseRes.x/2*1.28,baseRes.y/5 + baseRes.y*(550.0f/1080.0f));
    //hammer_upgrade->setScale(0.3,0.3);

    //Cargamos el arco
    bow_upgrade = new sf::RectangleShape();
    bow_upgrade->setTexture(resources->loadTexture("resources/bow.png"));
    bow_upgrade->setSize(sf::Vector2f(baseRes.x*0.0625f,baseRes.y*0.1111111f)); // 120x120 sobre 1920x1080
    bow_upgrade->setOrigin(bow_upgrade->getSize()/2.0f);
    bow_upgrade->setPosition(baseRes.x/2*1.53,baseRes.y/5 + baseRes.y*(550.0f/1080.0f));
    //bow_upgrade->setScale(0.3,0.3);

    // Boss Key Icons
    sf::Vector2f size = {baseRes.x*0.05f,baseRes.x*0.05f};
    float baseHeight = baseRes.y*0.82f;
    float baseXpos = baseRes.x*0.6f;
    float separation = baseRes.x*0.07f;
    mineKey = new sf::RectangleShape();
    mineKey->setTexture(resources->loadTexture("resources/bosskeyMine.png"));
    mineKey->setSize(size);
    mineKey->setPosition(baseXpos, baseHeight);
    cementeryKey = new sf::RectangleShape();
    cementeryKey->setTexture(resources->loadTexture("resources/bosskeyCementery.png"));
    cementeryKey->setSize(size);
    cementeryKey->setPosition(baseXpos+separation, baseHeight);
    towerKey = new sf::RectangleShape();
    towerKey->setTexture(resources->loadTexture("resources/bosskeyTower.png"));
    towerKey->setSize(size);
    towerKey->setPosition(baseXpos+separation*2, baseHeight);
}

void PauseState::initPlayerData()
{
    i_damage  = atoi(this->player->getSwordLvl().c_str());
    i_life    = atoi(this->player->getHealthUpg().c_str());
    i_kills   = atoi(this->player->getKills().c_str());
    i_money   = atoi(this->player->getMony().c_str());
    i_hammer  = atoi(this->player->getHammrLvl().c_str());
    i_score   = atoi(this->player->getScore().c_str());
    i_shield  = atoi(this->player->getShieldLvl().c_str());
    i_bow     = atoi(this->player->getBowLvl().c_str());

    bool keys[3];
    for(int i=0;i<3;i++)
        keys[i] = this->player->getKeyUnlocked(i)=="1"?true:false;

    if(keys[0]) mineKey->setFillColor(sf::Color::White);
    else        mineKey->setFillColor(sf::Color::Transparent);
    if(keys[1]) cementeryKey->setFillColor(sf::Color::White);
    else        cementeryKey->setFillColor(sf::Color::Transparent);
    if(keys[2]) towerKey->setFillColor(sf::Color::White);
    else        towerKey->setFillColor(sf::Color::Transparent);
}

void PauseState:: update()
{ 
    if(this->b_reInit)
    {
        this->b_reInit = false;
        this->initPlayerData();
    }

    Engine* engine = Engine::getInstance();
    if(engine->getKeyPressed(sf::Keyboard::Down)){
        if(seleccion == 3)
            seleccion = 1;
        else
            seleccion++;
    }
    if(engine->getKeyPressed(sf::Keyboard::Up)){
        if(seleccion == 1)
            seleccion = 3;
        else
            seleccion--;
    }

    //AUMENTO LAS ESTADISTICAS DE FORMA ARTIFICIAL PARA PROBAR COSAS
    if(engine->getKeyPressed(sf::Keyboard::A))
    {
        this->player->setMony(i_money + 10);
        i_money = atoi(this->player->getMony().c_str());
    }

    if(engine->getKeyPressed(sf::Keyboard::P) || engine->getKeyPressed(sf::Keyboard::Escape)){
        this->changeState(GAME_STATE, false);
    }

    if(engine->getKeyPressed(sf::Keyboard::Return)){
        switch(seleccion){
            case 1: //Volvemos a la partida
                this->changeState(GAME_STATE, false);
                break;

            case 2:  //Volvemos al menu principal
                this->changeState(MAINMENU_STATE, true);
                break;

            case 3:  // Reiniciamos el nivel
                this->changeState(GAME_STATE, true);
                break;
        }
        this->b_reInit = true;
    }

    // Arrow movement
    if(clock->getElapsedTime().asMilliseconds()>(0.004f)){  //0.00004
        int steps = clock->getElapsedTime().asSeconds() / 0.004f;
        prevArrowMove = arrowMove;
        if(dir==true)
            arrowMove+=(steps/30.0f);
        else 
            arrowMove-=(steps/30.0f);

        if(arrowMove>1.0f)arrowMove=1.0f;
        if(arrowMove<-1.0f)arrowMove=-1.0f;
        
        clock->restart();

        if(arrowMove>=1.0f) dir=false; else if(arrowMove<=-1.0f) dir = true;
    }
}

void PauseState:: render(float frameProgress){

    Engine* engine = Engine::getInstance();
    engine->setFollowView(false);
    //Adaptamos el filtro al tamaño de la pantalla
    //Dibujamos fondo y filtro en ese orden  !!!!IMPORTANTE: NO CAMBIAR EL ORDEN¡¡¡¡
    engine->renderDrawable(filtro);

    //Dibujamos las opciones del menu
    drawText();
    //Dibujamos las flechas selectoras
    drawArrow(frameProgress);
    //Dibujamos el pergamino
    drawParchment();
    //Dibujamos los datos SOBRE el pergamino
    drawPlayerData();

    // Dibujamos las llaves de jefe conseguidas
    engine->renderDrawable(mineKey);
    engine->renderDrawable(cementeryKey);
    engine->renderDrawable(towerKey);
}

void PauseState::drawText(){ 

    Engine* engine = Engine::getInstance();

    texto->setColor(sf::Color::White);

    texto->setString("Atras");
    texto->setPosition(engine->getBaseResolution().x/4,engine->getBaseResolution().y/4);
    texto->setCharacterSize(engine->getBaseResolution().x*0.028125f);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Menu");
    texto->setPosition(engine->getBaseResolution().x/4,engine->getBaseResolution().y*0.38888888f);
    texto->setCharacterSize(engine->getBaseResolution().x*0.028125f);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Cargar");
    texto->setPosition(engine->getBaseResolution().x/4,engine->getBaseResolution().y*0.52777777f);
    texto->setCharacterSize(engine->getBaseResolution().x*0.028125f);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);


}

void PauseState:: drawArrow(float frameProgress){

    Engine* engine = Engine::getInstance();

    float baseHeight = engine->getBaseResolution().y * 0.25926f;    // 280 sobre 1080
    float separationY = engine->getBaseResolution().y * 0.1388888f; // 150 sobre 1080
    switch(seleccion){
        case 1:
            flecha_selector->setPosition(engine->getBaseResolution().x*0.15625f, baseHeight);                     
            break;

        case 2:
            flecha_selector->setPosition(engine->getBaseResolution().x*0.15625f, baseHeight + separationY);
            break;

        case 3:
            flecha_selector->setPosition(engine->getBaseResolution().x*0.15625f, baseHeight + separationY*2);
            break;
    }

    float finalMove = this->prevArrowMove + (this->arrowMove - this->prevArrowMove) * frameProgress;
    finalMove *= engine->getBaseResolution().x * 0.015625f;
    flecha_selector->move(finalMove,0);

    engine->renderDrawable(flecha_selector);

}

void PauseState:: drawPlayerData(){

    Engine* engine = Engine::getInstance();
    sf::Vector2f baseRes = sf::Vector2f(engine->getBaseResolution().x,engine->getBaseResolution().y);

    texto->setColor(sf::Color::Black);

    //Score
    texto->setString("Score : " + std::to_string(i_score));
    texto->setPosition(baseRes.x/2*1.08,baseRes.y/5);
    texto->setCharacterSize(baseRes.x*0.028125f);
    texto->setOrigin(0,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    //Money
    engine->renderDrawable(coin_purse);

    texto->setString(" : " + std::to_string(i_money));
    texto->setPosition(baseRes.x/2*1.12,baseRes.y/5 + baseRes.y*(90.0f/1080.0f));
    texto->setCharacterSize(baseRes.x*0.028125f);
    texto->setOrigin(0,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    //Kills
    engine->renderDrawable(kill_count);

    texto->setString(" : " + std::to_string(i_kills));
    texto->setPosition(baseRes.x/2*1.16,baseRes.y/5 + baseRes.y*(190.0f/1080.0f));
    texto->setCharacterSize(baseRes.x*0.028125f);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);


    //Texto "Estadisticas"
    texto->setString(" Mejoras Actuales ");
    texto->setStyle(sf::Text::Underlined |sf::Text::Bold );
    texto->setPosition(baseRes.x/2*1.4,baseRes.y/5 + baseRes.y*(290.0f/1080.0f));
    texto->setCharacterSize(baseRes.x*0.028125f);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);
    texto->setStyle(sf::Text::Regular);

    //Life
    engine->renderDrawable(heart_upgrade);

    texto->setString("Lv " + (i_life >= maxLvImprovement ? "Max" : std::to_string(i_life)));
    texto->setPosition(baseRes.x/4*2*1.15,baseRes.y/5 + baseRes.y*(450.0f/1080.0f));
    texto->setCharacterSize(baseRes.x*0.02604f);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    //Sword
    if(i_damage != -1){
        engine->renderDrawable(sword_upgrade);

        texto->setString("Lv " + (i_damage >= maxLvImprovement ? "Max" : std::to_string(i_damage)));
        texto->setPosition(baseRes.x/2*1.4,baseRes.y/5 + baseRes.y*(450.0f/1080.0f));
        texto->setCharacterSize(baseRes.x*0.02604f);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }
    if(i_shield != -1){

        //Shield
        engine->renderDrawable(shield_upgrade);

        texto->setString("Lv Max");
        texto->setPosition(baseRes.x/2*1.65,baseRes.y/5 + baseRes.y*(450.0f/1080.0f));
        texto->setCharacterSize(baseRes.x*0.02604f);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }
    if(i_hammer != -1){
        //Hammer
        engine->renderDrawable(hammer_upgrade);

        texto->setString("Lv " + (i_hammer >= maxLvImprovement ? "Max" : std::to_string(i_hammer)));
        texto->setPosition(baseRes.x/2*1.28,baseRes.y/5 + baseRes.y*(600.0f/1080.0f));
        texto->setCharacterSize(baseRes.x*0.02604f);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    
    }

    if(i_bow != -1){
        //Bow
        engine->renderDrawable(bow_upgrade);

        texto->setString("Lv " + (i_bow >= maxLvImprovement ? "Max" : std::to_string(i_bow)));
        texto->setPosition(baseRes.x/2*1.53,baseRes.y/5 + baseRes.y*(600.0f/1080.0f));
        texto->setCharacterSize(baseRes.x*0.02604f);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }
}

void PauseState:: drawParchment()
{
    Engine* engine = Engine::getInstance();
    engine->renderDrawable(parchment);
} 
