#include "PauseState.h"


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
    delete armor_upgrade;
    delete parchment;
    delete clock;
}

void PauseState::loadAssets(){

        //Cargamos el filtro
    filtro = new sf::RectangleShape();
    filtro->setFillColor(sf::Color(0,0,0,120));

    //Cargamos la fuente de texto
    if (!fuente.loadFromFile("resources/XOX.ttf")) {
        std::cerr << "Error cargando la fuente XOX.png";
    exit(0);
    }
    texto= new sf::Text();
    texto->setFont(fuente);
    texto->setColor(sf::Color::White);

    //Cargamos la flecha
    if (!arrow.loadFromFile("resources/pixel-arrow-png-18.png")) {
            std::cerr << "Error cargando la imagen TheRifted.png";
        exit(0);
    }
    flecha_selector = new sf::Sprite(arrow);
    flecha_selector->rotate(180);
    flecha_selector->setScale(0.4,0.4);
    flecha_selector->setOrigin(flecha_selector->getLocalBounds().width/2.0f, flecha_selector->getLocalBounds().height/2.0f);

    //Cargamos la flecha
    if (!pergamino.loadFromFile("resources/parchment.png")) {
            std::cerr << "Error cargando la imagen parchment.png";
        exit(0);
    }
    parchment = new sf::Sprite(pergamino);
    parchment->setOrigin(parchment->getLocalBounds().width/2.0f, parchment->getLocalBounds().height/2.0f);
    parchment->rotate(90);

    //Cargamos las mejoras de vida
    if (!heart.loadFromFile("resources/health_upgrade.png")) {
            std::cerr << "Error cargando la imagen health_upgrade.png";
        exit(0);
    }
    heart_upgrade = new sf::Sprite(heart);
    heart_upgrade->setOrigin(heart_upgrade->getLocalBounds().width/2.0f, heart_upgrade->getLocalBounds().height/2.0f);

    //Cargamos las mejoras de espada
    if (!sword.loadFromFile("resources/sword_upgrade.png")) {
            std::cerr << "Error cargando la imagen sword_upgrade.png";
        exit(0);
    }
    sword_upgrade = new sf::Sprite(sword);
    sword_upgrade->setOrigin(sword_upgrade->getLocalBounds().width/2.0f, sword_upgrade->getLocalBounds().height/2.0f);

    //Cargamos las monedas
    if (!coin.loadFromFile("resources/coin.png")) {
            std::cerr << "Error cargando la imagen coin.png";
        exit(0);
    }
    coin_purse = new sf::Sprite(coin);
    coin_purse->setOrigin(coin_purse->getLocalBounds().width/2.0f, coin_purse->getLocalBounds().height/2.0f);

    //Cargamos las kills
    if (!skull.loadFromFile("resources/skull.png")) {
            std::cerr << "Error cargando la imagen skull.png";
        exit(0);
    }
    kill_count = new sf::Sprite(skull);
    kill_count->setOrigin(kill_count->getLocalBounds().width/2.0f, kill_count->getLocalBounds().height/2.0f);

    //Cargamos las armaduras
    if (!armor.loadFromFile("resources/armor_v1.png")) {
            std::cerr << "Error cargando la imagen armor_v1.png";
        exit(0);
    }
    armor_upgrade = new sf::Sprite(armor);
    armor_upgrade->setOrigin(armor_upgrade->getLocalBounds().width/2.0f, armor_upgrade->getLocalBounds().height/2.0f);

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

}

void PauseState:: render(){

    Engine* engine = Engine::getInstance();
    engine->setFollowView(false);
    //Adaptamos el filtro al tamaño de la pantalla
    filtro->setSize(sf::Vector2f(engine->getWindowSize().x,engine->getWindowSize().y));
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
    i_damage = atoi(this->player->getSwordLvl().c_str());
    i_life = atoi(this->player->getHealthUpg().c_str());
    i_kills = atoi(this->player->getKills().c_str());
    i_money = atoi(this->player->getMony().c_str());
    i_score = 0;//atoi(this->player->getKills().c_str());
    i_armor = 0;//atoi(this->player->getKills().c_str());
}

void PauseState::drawText(){ 

    Engine* engine = Engine::getInstance();

    texto->setColor(sf::Color::White);

    texto->setString("Ajustes");
    texto->setPosition(engine->getWindowSize().x/4,engine->getWindowSize().y/4);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Atras");
    texto->setPosition(engine->getWindowSize().x/4,engine->getWindowSize().y/4 + 150);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    texto->setString("Menu");
    texto->setPosition(engine->getWindowSize().x/4,engine->getWindowSize().y/4 + 300);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);


}

void PauseState:: drawArrow(){

     Engine* engine = Engine::getInstance();

        switch(seleccion){
            case 1:
                flecha_selector->setPosition(engine->getWindowSize().x/4 - 180 ,engine->getWindowSize().y/4+ 10);                     
            break;

            case 2:
                flecha_selector->setPosition(engine->getWindowSize().x/4 - 180 ,engine->getWindowSize().y/4+ 160);
            break;

            case 3:
                flecha_selector->setPosition(engine->getWindowSize().x/4 - 180 ,engine->getWindowSize().y/4+ 310);
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

    texto->setString("Score : " + std::to_string(i_score));
    texto->setPosition(engine->getWindowSize().x/4*2.7,engine->getWindowSize().y/5);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    heart_upgrade->setPosition(engine->getWindowSize().x/4*2.6,engine->getWindowSize().y/5 + 110);
    heart_upgrade->setScale(0.1,0.1);
    engine->renderDrawable(heart_upgrade);

    texto->setString("Lv " + std::to_string(i_life));
    texto->setPosition(engine->getWindowSize().x/4*3,engine->getWindowSize().y/5 + 90);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    sword_upgrade->setPosition(engine->getWindowSize().x/4*2.6,engine->getWindowSize().y/5 + 230);
    sword_upgrade->setScale(0.1,0.1);
    engine->renderDrawable(sword_upgrade);

    texto->setString("Lv " + std::to_string(i_damage));
    texto->setPosition(engine->getWindowSize().x/4*3,engine->getWindowSize().y/5 + 210);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    armor_upgrade->setPosition(engine->getWindowSize().x/4*2.6,engine->getWindowSize().y/5 + 350);
    armor_upgrade->setScale(1,1);
    engine->renderDrawable(armor_upgrade);

    texto->setString("Lv " + std::to_string(i_armor));
    texto->setPosition(engine->getWindowSize().x/4*3,engine->getWindowSize().y/5 + 330);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    coin_purse->setPosition(engine->getWindowSize().x/4*2.6,engine->getWindowSize().y/5 + 470);
    coin_purse->setScale(0.1,0.1);
    engine->renderDrawable(coin_purse);

    texto->setString(" " + std::to_string(i_money));
    texto->setPosition(engine->getWindowSize().x/4*3,engine->getWindowSize().y/5 + 450);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    kill_count->setPosition(engine->getWindowSize().x/4*2.6,engine->getWindowSize().y/5 + 590);
    kill_count->setScale(0.1,0.1);
    engine->renderDrawable(kill_count);

    texto->setString(" " + std::to_string(i_kills));
    texto->setPosition(engine->getWindowSize().x/4*3,engine->getWindowSize().y/5 + 570);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

}

void PauseState:: drawParchment(){

    Engine* engine = Engine::getInstance();

    parchment->setPosition(engine->getWindowSize().x/4*2.8,engine->getWindowSize().y/5 + 260);
    parchment->setScale(0.85,0.7);
    engine->renderDrawable(parchment);

} 
