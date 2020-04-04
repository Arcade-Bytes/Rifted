#include "PauseState.h"

#define maxLvImprovement 3  //Nivel Maximo de mejora

PauseState::PauseState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = PAUSE_STATE;

    //Rellenamos variables
    this->initPlayerData();

    //Establecemos donde empieza la flecha
    seleccion = 1;
    //Inicializamos el reloj
    clock = new sf::Clock();
    //Paramentros para la animacion de la flecha
    dir = true;
    i = 0;

    loadAssets();
}

PauseState::~PauseState(){

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
    delete clock;
}

void PauseState::loadAssets(){

    //Cargamos el filtro
    filtro = new sf::RectangleShape();
    filtro->setFillColor(sf::Color(0,0,0,120));

    ResourceManager* resources = ResourceManager::getInstance();

    //Cargamos la fuente de texto
    texto= new sf::Text();
    texto->setFont(*resources->loadFont("resources/XOX.ttf"));
    texto->setColor(sf::Color::White);

    //Cargamos la flecha
    flecha_selector = new sf::Sprite(*resources->loadTexture("resources/pixel-arrow-png-18.png"));
    flecha_selector->rotate(180);
    flecha_selector->setScale(0.4,0.4);
    flecha_selector->setOrigin(flecha_selector->getLocalBounds().width/2.0f, flecha_selector->getLocalBounds().height/2.0f);

    //Cargamos el pergamino
    parchment = new sf::Sprite(*resources->loadTexture("resources/parchment.png"));
    parchment->setOrigin(parchment->getLocalBounds().width/2.0f, parchment->getLocalBounds().height/2.0f);
    parchment->rotate(90);

    //Cargamos las mejoras de vida
    heart_upgrade = new sf::Sprite(*resources->loadTexture("resources/health_upgrade.png"));
    heart_upgrade->setOrigin(heart_upgrade->getLocalBounds().width/2.0f, heart_upgrade->getLocalBounds().height/2.0f);

    //Cargamos las mejoras de espada
    sword_upgrade = new sf::Sprite(*resources->loadTexture("resources/sword_upgrade.png"));
    sword_upgrade->setOrigin(sword_upgrade->getLocalBounds().width/2.0f, sword_upgrade->getLocalBounds().height/2.0f);

    //Cargamos las monedas
    coin_purse = new sf::Sprite(*resources->loadTexture("resources/coin.png"));
    coin_purse->setOrigin(coin_purse->getLocalBounds().width/2.0f, coin_purse->getLocalBounds().height/2.0f);

    //Cargamos las kills
    kill_count = new sf::Sprite(*resources->loadTexture("resources/skull.png"));
    kill_count->setOrigin(kill_count->getLocalBounds().width/2.0f, kill_count->getLocalBounds().height/2.0f);

    //Cargamos el escudo
    shield_upgrade = new sf::Sprite(*resources->loadTexture("resources/shield.png"));
    shield_upgrade->setOrigin(shield_upgrade->getLocalBounds().width/2.0f, shield_upgrade->getLocalBounds().height/2.0f);

    //Cargamos el martillo
    hammer_upgrade = new sf::Sprite(*resources->loadTexture("resources/hammer.png"));
    hammer_upgrade->setOrigin(hammer_upgrade->getLocalBounds().width/2.0f, hammer_upgrade->getLocalBounds().height/2.0f);

    //Cargamos el arco
    bow_upgrade = new sf::Sprite(*resources->loadTexture("resources/bow.png"));
    bow_upgrade->setOrigin(bow_upgrade->getLocalBounds().width/2.0f, bow_upgrade->getLocalBounds().height/2.0f);

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
        this->player->setMony(i_money + 100);
        i_money = atoi(this->player->getMony().c_str());
    }

    if(engine->getKeyPressed(sf::Keyboard::P)){
        this->changeState(GAME_STATE, false);
    }

    if(engine->getKeyPressed(sf::Keyboard::Return)){
        switch(seleccion){
            case 1: //Aqui ira el hipotético ajustes
            break;

            case 2: //Volvemos a la partida
                this->changeState(GAME_STATE, false);
                break;

            case 3:  //Volvemos al menu principal
                this->changeState(MAINMENU_STATE, true);
                break;
        }
        this->b_reInit = true;
    }

    if(engine->getKeyPressed(sf::Keyboard::B)){

        if(i_bow != 0)
            i_bow = 0;
        else i_bow = -1;

        if(i_hammer != 0)
            i_hammer = 0;
        else i_hammer = -1;

        if(i_shield != 0)
            i_shield = 0;
        else i_shield = -1;

    }

}

void PauseState:: render(){

    Engine* engine = Engine::getInstance();
    engine->setFollowView(false);
    //Adaptamos el filtro al tamaño de la pantalla
    filtro->setSize(sf::Vector2f(engine->getBaseResolution().x,engine->getBaseResolution().y));
    //Dibujamos fondo y filtro en ese orden  !!!!IMPORTANTE: NO CAMBIAR EL ORDEN¡¡¡¡
    engine->renderDrawable(filtro);

    //Dibujamos las opciones del menu
    drawText();
    //Dibujamos las flechas selectoras
    drawArrow();
    //Dibujamos el pergamino
    drawParchment();
    //Dibujamos los datos SOBRE el pergamino
    drawPlayerData();

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
}

void PauseState::drawText(){ 

    Engine* engine = Engine::getInstance();

    texto->setColor(sf::Color::White);

    texto->setString("Ajustes");
    texto->setPosition(engine->getBaseResolution().x/4,engine->getBaseResolution().y/4);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Atras");
    texto->setPosition(engine->getBaseResolution().x/4,engine->getBaseResolution().y/4 + 150);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Menu");
    texto->setPosition(engine->getBaseResolution().x/4,engine->getBaseResolution().y/4 + 300);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);


}

void PauseState:: drawArrow(){

     Engine* engine = Engine::getInstance();

        switch(seleccion){
            case 1:
                flecha_selector->setPosition(engine->getBaseResolution().x/4 - 180 ,engine->getBaseResolution().y/4+ 10);                     
            break;

            case 2:
                flecha_selector->setPosition(engine->getBaseResolution().x/4 - 180 ,engine->getBaseResolution().y/4+ 160);
            break;

            case 3:
                flecha_selector->setPosition(engine->getBaseResolution().x/4 - 180 ,engine->getBaseResolution().y/4+ 310);
            break;
        }

       if(clock->getElapsedTime().asSeconds()>(0.00004)){

            if(dir==true)
                i++;
            
            else i--;

            flecha_selector->move(i,0);
            
            clock->restart();

            if(i==30) dir=false; else if(i==-30) dir = true;

        }

        engine->renderDrawable(flecha_selector);

}

void PauseState:: drawPlayerData(){

    Engine* engine = Engine::getInstance();

    texto->setColor(sf::Color::Black);

    //Score

    texto->setString("Score : " + std::to_string(i_score));
    texto->setPosition(engine->getBaseResolution().x/2*1.08,engine->getBaseResolution().y/5);
    texto->setCharacterSize(54);
    texto->setOrigin(0,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);


    //Money

    coin_purse->setPosition(engine->getBaseResolution().x/2*1.1,engine->getBaseResolution().y/5 + 110);
    coin_purse->setScale(0.1,0.1);
    engine->renderDrawable(coin_purse);

    texto->setString(" : " + std::to_string(i_money));
    texto->setPosition(engine->getBaseResolution().x/2*1.12,engine->getBaseResolution().y/5 + 90);
    texto->setCharacterSize(54);
    texto->setOrigin(0,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    //Kills

    kill_count->setPosition(engine->getBaseResolution().x/2*1.1,engine->getBaseResolution().y/5 + 210);
    kill_count->setScale(0.1,0.1);
    engine->renderDrawable(kill_count);

    texto->setString(" : " + std::to_string(i_kills));
    texto->setPosition(engine->getBaseResolution().x/2*1.16,engine->getBaseResolution().y/5 + 190);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);


    //Texto "Estadisticas"

    texto->setString(" Mejoras Actuales ");
    texto->setStyle(sf::Text::Underlined |sf::Text::Bold );
    texto->setPosition(engine->getBaseResolution().x/2*1.4,engine->getBaseResolution().y/5 + 290);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);
    texto->setStyle(sf::Text::Regular);

    //Life

    heart_upgrade->setPosition(engine->getBaseResolution().x/2*1.15,engine->getBaseResolution().y/5 + 410);
    heart_upgrade->setScale(0.1,0.1);
    engine->renderDrawable(heart_upgrade);

    texto->setString("Lv " + (i_life >= maxLvImprovement ? "Max" : std::to_string(i_life)));
    texto->setPosition(engine->getBaseResolution().x/4*2*1.15,engine->getBaseResolution().y/5 + 450);
    texto->setCharacterSize(50);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    //Sword

    if(i_damage != -1){
        sword_upgrade->setPosition(engine->getBaseResolution().x/2*1.4,engine->getBaseResolution().y/5 + 410);
        sword_upgrade->setScale(0.13,0.13);
        engine->renderDrawable(sword_upgrade);

        texto->setString("Lv " + (i_damage >= maxLvImprovement ? "Max" : std::to_string(i_damage)));
        texto->setPosition(engine->getBaseResolution().x/2*1.4,engine->getBaseResolution().y/5 + 450);
        texto->setCharacterSize(50);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }
    if(i_shield != -1){

        //Shield

        shield_upgrade->setPosition(engine->getBaseResolution().x/2*1.65,engine->getBaseResolution().y/5 + 400);
        shield_upgrade->setScale(0.3,0.3);
        engine->renderDrawable(shield_upgrade);

        texto->setString("Lv Max");
        texto->setPosition(engine->getBaseResolution().x/2*1.65,engine->getBaseResolution().y/5 + 450);
        texto->setCharacterSize(50);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }
    if(i_hammer != -1){
        //Hammer

        hammer_upgrade->setPosition(engine->getBaseResolution().x/2*1.28,engine->getBaseResolution().y/5 + 550);
        hammer_upgrade->setScale(0.3,0.3);
        engine->renderDrawable(hammer_upgrade);

        texto->setString("Lv " + (i_hammer >= maxLvImprovement ? "Max" : std::to_string(i_hammer)));
        texto->setPosition(engine->getBaseResolution().x/2*1.28,engine->getBaseResolution().y/5 + 600);
        texto->setCharacterSize(50);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    
    }

    if(i_bow != -1){

        //Bow

        bow_upgrade->setPosition(engine->getBaseResolution().x/2*1.53,engine->getBaseResolution().y/5 + 550);
        bow_upgrade->setScale(0.3,0.3);
        engine->renderDrawable(bow_upgrade);

        texto->setString("Lv " + (i_bow >= maxLvImprovement ? "Max" : std::to_string(i_bow)));
        texto->setPosition(engine->getBaseResolution().x/2*1.53,engine->getBaseResolution().y/5 + 600);
        texto->setCharacterSize(50);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }

}

void PauseState:: drawParchment(){

    Engine* engine = Engine::getInstance();

    parchment->setPosition(engine->getBaseResolution().x/4*2.8,engine->getBaseResolution().y/5 + 260);
    parchment->setScale(0.85,0.7);
    engine->renderDrawable(parchment);

} 
