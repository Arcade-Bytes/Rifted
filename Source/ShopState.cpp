#include "ShopState.h"

#define maxLvImprovement 5  //Nivel Maximo de mejora
//Los precios bases de la tienda
#define basePriceDmg 100    // Precio de la espada
#define basePriceLife 100   // Precio de la vida
#define basePriceArmor 100  // Precio de la armadura


ShopState::ShopState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = SHOP_STATE;
    b_reInit = true;

    this->initPlayerData();

    //Establecemos donde empieza la flecha
    seleccion = 1;
    //Inicializamos el reloj
    clock = new sf::Clock();
    //Paramentros para la animacion de la flecha
    dir = true;
    i = 0;

    loadAssets();
    loadDialogue();
}

ShopState::~ShopState(){

    delete parchment;
    delete texto; 
    delete flecha_selector;
    delete heart_upgrade;
    delete sword_upgrade;
    delete coin_purse;
    delete armor_upgrade;
    delete npc_cleric;
    delete text_box;
    delete clock;
}

void ShopState:: loadDialogue(){

    //Preparamos textos de la tienda
    pointer.setCharacterSize(35);
        //Opciones de bienvenida
    pointer.setString(" Welcome again ");
    textoTienda_v1.push_back(pointer);
    pointer.setString(" Fancy something ? ");
    textoTienda_v1.push_back(pointer);
    pointer.setString(" You better have money ");
    textoTienda_v1.push_back(pointer);
    pointer.setString(" Something new ? ");
    textoTienda_v1.push_back(pointer);
        //Opciones para compra ok
    pointer.setString(" Enjoy it ");
    textoTienda_v2.push_back(pointer);
    pointer.setString(" Make good use of it ");
    textoTienda_v2.push_back(pointer);
    pointer.setString(" Something else ? ");
    textoTienda_v2.push_back(pointer);

        //Opciones para compra mal
    pointer.setString(" Don't think so ");
    textoTienda_v3.push_back(pointer);
    pointer.setString(" I don't give freebies ");
    textoTienda_v3.push_back(pointer);
    pointer.setString(" Maybe later ");
    textoTienda_v3.push_back(pointer);

       //Opciones objeto al máximo
    pointer.setString(" Don't have more ");
    textoTienda_v4.push_back(pointer);
    pointer.setString(" It's enought ");
    textoTienda_v4.push_back(pointer);
    pointer.setString(" You are at your limit ");
    textoTienda_v4.push_back(pointer); 

        //Opciones de despedida
    pointer.setString(" See you ");
    textoTienda_v5.push_back(pointer);
    pointer.setString(" Don't get killed ");
    textoTienda_v5.push_back(pointer);
    pointer.setString(" Bring money next time ");
    textoTienda_v5.push_back(pointer);

    texto_render=textoTienda_v1[random()%textoTienda_v1.size()];
    texto_render.setColor(sf::Color::White);

}

void ShopState:: loadAssets(){

    //Cargamos la fuente de texto
    if (!fuente.loadFromFile("resources/XOX.ttf")) {
        std::cerr << "Error cargando la fuente XOX.png";
    exit(0);
    }
    texto= new sf::Text();
    texto->setFont(fuente);
    texto->setColor(sf::Color::White);
    texto_render.setFont(fuente);
    pointer.setFont(fuente);

    //Cargamos la flecha
    if (!arrow.loadFromFile("resources/black_arrow.png")) {
            std::cerr << "Error cargando la imagen black_arrow.png";
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

    //Cargamos la armadura
    if (!armor.loadFromFile("resources/armor_v1.png")) {
            std::cerr << "Error cargando la imagen armor_v1.png";
        exit(0);
    }
    armor_upgrade = new sf::Sprite(armor);
    armor_upgrade->setOrigin(armor_upgrade->getLocalBounds().width/2.0f, armor_upgrade->getLocalBounds().height/2.0f);

    //Cargamos al tendero
    if (!cleric.loadFromFile("resources/cleric.png")) {
            std::cerr << "Error cargando la imagen cleric.png";
        exit(0);
    }
    npc_cleric = new sf::Sprite(cleric);
    npc_cleric->setOrigin(npc_cleric->getLocalBounds().width/2.0f, npc_cleric->getLocalBounds().height/2.0f);

    //Cargamos la caja de texto
    if (!box.loadFromFile("resources/text_box.png")) {
            std::cerr << "Error cargando la imagen text_box.png";
        exit(0);
    }
    text_box = new sf::Sprite(box);
    text_box->setOrigin(text_box->getLocalBounds().width/2.0f, text_box->getLocalBounds().height/2.0f);


}

void ShopState:: update(){
    if(this->b_reInit)
    {
        this->b_reInit = false;
        this->initPlayerData();
    }

    Engine* engine = Engine::getInstance();
    if(engine->getKeyPressed(sf::Keyboard::Right)){
        if(seleccion == 4)
            seleccion = 1;
        else
            seleccion++;
    }
    if(engine->getKeyPressed(sf::Keyboard::Left)){
        if(seleccion == 1)
            seleccion = 4;
        else
            seleccion--;
    }

    if(engine->getKeyPressed(sf::Keyboard::Return)){
        switch(seleccion){

            case 1: //SI LE DA AL BOTON DE SALIR ....
                texto_render = textoTienda_v5[random()%textoTienda_v5.size()]; //CARGAMOS UNA DESPEDIDA
                this->b_reInit = true;
                this->changeState(GAME_STATE);
            break;

            case 3: //SI COMPRA DANYO .....
                if(i_damage < maxLvImprovement){

                    if(i_damage*basePriceDmg <= i_money){
                        texto_render = textoTienda_v2[random()%textoTienda_v2.size()]; //CARGAMOS UN COMPRA CORRECTA Y LEVEL UP
                        i_money-=(i_damage*basePriceDmg);
                        i_damage+=1;
                    }else {texto_render = textoTienda_v3[random()%textoTienda_v3.size()];} //CARGAMOS COMPRA INCORRECTA
                }else{ texto_render = textoTienda_v4[random()%textoTienda_v4.size()];} //CARGAMOS ESTA AL MAXIMO
            break;
            
            case 2: //SI COMPRA VIDA .....

                if(i_life < maxLvImprovement){

                    if(i_life*basePriceLife <= i_money){
                        texto_render = textoTienda_v2[random()%textoTienda_v2.size()];
                        i_money-=(i_life*basePriceLife);
                        i_life+=1;
                    }else {texto_render = textoTienda_v3[random()%textoTienda_v3.size()];}
                }else{ texto_render = textoTienda_v4[random()%textoTienda_v4.size()];}
            
            break;
            
            case 4: 
                if(i_armor < maxLvImprovement){

                    if(i_armor*basePriceArmor <= i_money){
                        texto_render = textoTienda_v2[random()%textoTienda_v2.size()];
                        i_money-=(i_armor*basePriceArmor);
                        i_armor+=1;
                    }else {texto_render = textoTienda_v3[random()%textoTienda_v3.size()];}
                }else{ texto_render = textoTienda_v4[random()%textoTienda_v4.size()];}
        
            break;
        }
        this->player->setSwordLvl(i_damage);
        this->player->setHealthUpg(i_damage);
        this->player->setMony(i_money);
    }
}

void ShopState:: render(){
    Engine::getInstance()->setFollowView(false);
    drawParchment();
    drawPlayerData();
    drawText();
    drawArrow();
    drawPrices();
}

void ShopState::initPlayerData()
{
    i_damage = atoi(this->player->getSwordLvl().c_str());
    i_life = atoi(this->player->getHealthUpg().c_str());
    i_money = atoi(this->player->getMony().c_str());
    i_armor = 0;//atoi(this->player->getMony().c_str());
}

void ShopState::drawText(){

    Engine* engine = Engine::getInstance();

    //Cuadro de texto
    texto_render.setPosition(engine->getWindowSize().x/8+30,engine->getWindowSize().y/4);
    texto_render.setOrigin(texto_render.getLocalBounds().width/2.0f,texto_render.getLocalBounds().height/2.0f);
    engine->renderDrawable(&texto_render);

    //Boton atras
    texto->setString("Exit");
    texto->setPosition(engine->getWindowSize().x/4*2-120,engine->getWindowSize().y/8);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

}

void ShopState:: drawArrow(){

    Engine* engine = Engine::getInstance();

    switch(seleccion){
            case 1:

                flecha_selector->setPosition(engine->getWindowSize().x/4 + 180,engine->getWindowSize().y/8+10);                     
            
            break;

            case 2:

                flecha_selector->setPosition(engine->getWindowSize().x/4*1.45 -20,engine->getWindowSize().y/5 + 330);
            
            break;

            case 3:

                flecha_selector->setPosition(engine->getWindowSize().x/4*2.1 -20,engine->getWindowSize().y/5 + 330);

            break;

            case 4:

                flecha_selector->setPosition(engine->getWindowSize().x/4*2.75 -20 ,engine->getWindowSize().y/5+ 330);

            break;
        }
    if(seleccion==1){
       if(clock->getElapsedTime().asSeconds()>(0.00004)){

            if(dir==true)
                i++;
            
            else i--;

            flecha_selector->move(i,0);
            
            clock->restart();

            if(i==30) dir=false; else if(i==-30) dir = true;

        }
    }

        engine->renderDrawable(flecha_selector);

}

void ShopState:: drawPlayerData(){

    Engine* engine = Engine::getInstance();

    texto->setColor(sf::Color::Black);

    coin_purse->setPosition(engine->getWindowSize().x/8 *5.5,engine->getWindowSize().y/8);
    coin_purse->setScale(0.2,0.2);
    engine->renderDrawable(coin_purse);

    texto->setString(" " + std::to_string(i_money));
    texto->setPosition(engine->getWindowSize().x/8 * 6 + 100,engine->getWindowSize().y/8 + -20);
    texto->setCharacterSize(72);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

}

void ShopState:: drawParchment(){

    Engine* engine = Engine::getInstance();
    
    parchment->setPosition(engine->getWindowSize().x/2+200,engine->getWindowSize().y/2);
    parchment->setScale(1.5,1.2);
    engine->renderDrawable(parchment);

    npc_cleric->setPosition(engine->getWindowSize().x/10+60,engine->getWindowSize().y/10*8-50);
    npc_cleric->setScale(2,2);
    engine->renderDrawable(npc_cleric);

    text_box->setPosition(engine->getWindowSize().x/10+70,engine->getWindowSize().y/10*3-50);
    text_box->setScale(1,2);
    engine->renderDrawable(text_box);

    heart_upgrade->setPosition(engine->getWindowSize().x/4*1.75,engine->getWindowSize().y/5 + 210);
    heart_upgrade->setScale(0.15,0.15);
    engine->renderDrawable(heart_upgrade);


    if(i_life<maxLvImprovement)
        texto->setString("Lv " + std::to_string(i_life));
    else texto->setString("Lv Max ");
    texto->setPosition(engine->getWindowSize().x/4*1.75,engine->getWindowSize().y/5 + 310);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    sword_upgrade->setPosition(engine->getWindowSize().x/4*2.4,engine->getWindowSize().y/5 + 200);
    sword_upgrade->setScale(0.2,0.2);
    engine->renderDrawable(sword_upgrade);


    if(i_damage<maxLvImprovement)
        texto->setString("Lv " + std::to_string(i_damage));
    else texto->setString("Lv Max ");
    texto->setPosition(engine->getWindowSize().x/4*2.4,engine->getWindowSize().y/5 + 310);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    armor_upgrade->setPosition(engine->getWindowSize().x/4*3.05,engine->getWindowSize().y/5 + 200);
    armor_upgrade->setScale(2.5,2.5);
    engine->renderDrawable(armor_upgrade);


    if(i_armor<maxLvImprovement)
        texto->setString("Lv " + std::to_string(i_armor));
    else texto->setString("Lv Max ");
    texto->setPosition(engine->getWindowSize().x/4*3.05,engine->getWindowSize().y/5 + 310);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

}

void ShopState:: drawPrices(){

    Engine* engine = Engine::getInstance();

    if(i_life<maxLvImprovement){

        coin_purse->setPosition(engine->getWindowSize().x/4*1.75-60,engine->getWindowSize().y/5 + 378);
        coin_purse->setScale(0.1,0.1);
        engine->renderDrawable(coin_purse);

        texto->setString(std::to_string(basePriceLife*(i_life)));
        texto->setPosition(engine->getWindowSize().x/4*1.75 +20,engine->getWindowSize().y/5 + 360);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }

    if(i_damage<maxLvImprovement){

        coin_purse->setPosition(engine->getWindowSize().x/4*2.4-60,engine->getWindowSize().y/5 + 378);
        coin_purse->setScale(0.1,0.1);
        engine->renderDrawable(coin_purse);

        texto->setString(std::to_string(basePriceDmg*(i_damage)));
        texto->setPosition(engine->getWindowSize().x/4*2.4 +20,engine->getWindowSize().y/5 + 360);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }

    if(i_armor<maxLvImprovement){

        coin_purse->setPosition(engine->getWindowSize().x/4*3.05-60,engine->getWindowSize().y/5 + 378);
        coin_purse->setScale(0.1,0.1);
        engine->renderDrawable(coin_purse);

        armor_upgrade->setPosition(engine->getWindowSize().x/4*3.05-60,engine->getWindowSize().y/5 + 378);
        armor_upgrade->setScale(0.1,0.1);
        engine->renderDrawable(coin_purse);

        texto->setString(std::to_string(basePriceArmor*(i_armor)));
        texto->setPosition(engine->getWindowSize().x/4*3.05 +20,engine->getWindowSize().y/5 + 360);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }

}
