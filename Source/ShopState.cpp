#include "ShopState.h"

#define maxLvImprovement 3  //Nivel Maximo de mejora
//Los precios bases de la tienda
#define basePriceDmg 4  // Precio de la espada
#define basePriceLife 4   // Precio de la vida
#define basePriceArmor 4  // Precio de la armadura



ShopState::ShopState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = SHOP_STATE;

    this->initPlayerData();

    //Establecemos donde empieza la flecha
    seleccion = 1;
    //Inicializamos el reloj
    clock = new sf::Clock();
    //Paramentros para la animacion de la flecha
    dir = true;
    i = 0;
    max_seleccion = 2;//Vida y money nunca seran -1

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
    delete shield_upgrade;
    delete bow_upgrade;
    delete hammer_upgrade;
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
    ResourceManager* resources = ResourceManager::getInstance();

    //Cargamos la fuente de texto
    texto= new sf::Text();
    texto->setFont(*resources->loadFont("resources/XOX.ttf"));
    texto->setColor(sf::Color::White);
    texto_render.setFont(*resources->loadFont("resources/XOX.ttf"));
    pointer.setFont(*resources->loadFont("resources/XOX.ttf"));

    //Cargamos la flecha
    flecha_selector = new sf::Sprite(*resources->loadTexture("resources/black_arrow.png"));
    flecha_selector->rotate(180);
    flecha_selector->setScale(0.4,0.4);
    flecha_selector->setOrigin(flecha_selector->getLocalBounds().width/2.0f, flecha_selector->getLocalBounds().height/2.0f);

    //Cargamos la flecha
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

    //Cargamos el escudo
    shield_upgrade = new sf::Sprite(*resources->loadTexture("resources/shield.png"));
    shield_upgrade->setOrigin(shield_upgrade->getLocalBounds().width/2.0f, shield_upgrade->getLocalBounds().height/2.0f);

    //Cargamos el martillo
    hammer_upgrade = new sf::Sprite(*resources->loadTexture("resources/hammer.png"));
    hammer_upgrade->setOrigin(hammer_upgrade->getLocalBounds().width/2.0f, hammer_upgrade->getLocalBounds().height/2.0f);

    //Cargamos el arco
    bow_upgrade = new sf::Sprite(*resources->loadTexture("resources/bow.png"));
    bow_upgrade->setOrigin(bow_upgrade->getLocalBounds().width/2.0f, bow_upgrade->getLocalBounds().height/2.0f);

    //Cargamos al tendero
    npc_cleric = new sf::Sprite(*resources->loadTexture("resources/cleric.png"));
    npc_cleric->setOrigin(npc_cleric->getLocalBounds().width/2.0f, npc_cleric->getLocalBounds().height/2.0f);

    //Cargamos la caja de texto
    text_box = new sf::Sprite(*resources->loadTexture("resources/text_box.png"));
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
        this->selectionUp();
        while(!getIsUnlocked(this->seleccion))
        {
            this->selectionUp();
        }
    }
    if(engine->getKeyPressed(sf::Keyboard::Left)){
        this->selectionDown();
        while(!getIsUnlocked(this->seleccion))
        {
            this->selectionDown();
        }
    }

    if(engine->getKeyPressed(sf::Keyboard::Return)){
        switch(seleccion){

            case 1: //SI LE DA AL BOTON DE SALIR ....
                texto_render = textoTienda_v5[random()%textoTienda_v5.size()]; //CARGAMOS UNA DESPEDIDA
                this->b_reInit = true;
                this->changeState(GAME_STATE, false);
            break;
            
            case 2: //SI COMPRA VIDA .....

                if(i_life < maxLvImprovement){

                    if((i_life+1)*basePriceLife <= i_money){
                        texto_render = textoTienda_v2[random()%textoTienda_v2.size()];
                        i_money-=((i_life+1)*basePriceLife);
                        i_life+=1;
                    }else {texto_render = textoTienda_v3[random()%textoTienda_v3.size()];}
                }else{ texto_render = textoTienda_v4[random()%textoTienda_v4.size()];}
            
            break;

            case 3: //SI COMPRA DANYO .....
                if(i_damage < maxLvImprovement){

                    if((i_damage+1)*basePriceDmg <= i_money && i_damage >= 0){
                        texto_render = textoTienda_v2[random()%textoTienda_v2.size()]; //CARGAMOS UN COMPRA CORRECTA Y LEVEL UP
                        i_money-=((i_damage+1)*basePriceDmg);
                        i_damage+=1;
                    }else {texto_render = textoTienda_v3[random()%textoTienda_v3.size()];} //CARGAMOS COMPRA INCORRECTA
                }else{ texto_render = textoTienda_v4[random()%textoTienda_v4.size()];} //CARGAMOS ESTA AL MAXIMO
            break;
            
            case 4: 
                if(i_shield < 0){ // Shield not upgradable

                    if((i_shield+1)*basePriceArmor <= i_money && i_shield >= 0){
                        texto_render = textoTienda_v2[random()%textoTienda_v2.size()];
                        i_money-=((i_shield+1)*basePriceArmor);
                        i_shield+=1;
                    }else {texto_render = textoTienda_v3[random()%textoTienda_v3.size()];}
                }else{ texto_render = textoTienda_v4[random()%textoTienda_v4.size()];}
        
            break;

            case 5: 
                if(i_hammer < maxLvImprovement){

                    if((i_hammer+1)*basePriceArmor <= i_money && i_hammer >= 0){
                        texto_render = textoTienda_v2[random()%textoTienda_v2.size()];
                        i_money-=((i_hammer+1)*basePriceArmor);
                        i_hammer+=1;
                    }else {texto_render = textoTienda_v3[random()%textoTienda_v3.size()];}
                }else{ texto_render = textoTienda_v4[random()%textoTienda_v4.size()];}
        
            break;

            case 6: 
                if(i_bow < maxLvImprovement){

                    if((i_bow+1)*basePriceArmor <= i_money && i_bow >= 0){
                        texto_render = textoTienda_v2[random()%textoTienda_v2.size()];
                        i_money-=((i_bow+1)*basePriceArmor);
                        i_bow+=1;
                    }else {texto_render = textoTienda_v3[random()%textoTienda_v3.size()];}
                }else{ texto_render = textoTienda_v4[random()%textoTienda_v4.size()];}
        
            break;
        }
        this->player->setMony(i_money);
        this->player->setSwordLvl(i_damage);
        this->player->setHealthUpg(i_life);
        this->player->setShieldLvl(i_shield);
        this->player->setHammrLvl(i_hammer);
        this->player->setBowLvl(i_bow);
    }
}

void ShopState:: render(float frameProgress){
    Engine::getInstance()->setFollowView(false);
    drawParchment();
    drawPlayerData();
    drawText();
    drawArrow();
    drawPrices();
}

void ShopState::initPlayerData()
{
    i_damage  = atoi(this->player->getSwordLvl().c_str());
    i_life    = atoi(this->player->getHealthUpg().c_str());
    i_money   = atoi(this->player->getMony().c_str());
    i_hammer  = atoi(this->player->getHammrLvl().c_str());
    i_bow     = atoi(this->player->getBowLvl().c_str());
    i_shield  = atoi(this->player->getShieldLvl().c_str());
    this->max_seleccion = 6;
}

bool ShopState::getIsUnlocked(int index)
{
    // Vida danyo escudo martillo bow
    bool unlocked = false;
    switch(index) {
        case 1: unlocked = true; break;
        case 2: if(i_life >= 0) unlocked = true; break;
        case 3: if(i_damage >= 0) unlocked = true; break;
        case 4: if(i_shield >= 0) unlocked = true; break;
        case 5: if(i_hammer >= 0) unlocked = true; break;
        case 6: if(i_bow >= 0) unlocked = true; break;
    }
    return unlocked;
}

void ShopState::selectionDown()
{
    if(seleccion == 1)
        seleccion = max_seleccion;
    else
        seleccion--;
}

void ShopState::selectionUp()
{
    if(seleccion == max_seleccion)
        seleccion = 1;
    else
        seleccion++;
}

void ShopState::drawText(){

    Engine* engine = Engine::getInstance();

    //Cuadro de texto
    texto_render.setPosition(engine->getBaseResolution().x/8+30,engine->getBaseResolution().y/4);
    texto_render.setOrigin(texto_render.getLocalBounds().width/2.0f,texto_render.getLocalBounds().height/2.0f);
    engine->renderDrawable(&texto_render);

    //Boton atras
    texto->setString("Exit");
    texto->setPosition(engine->getBaseResolution().x/4*2-120,engine->getBaseResolution().y/8);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

}

void ShopState:: drawArrow(){

    Engine* engine = Engine::getInstance();

    switch(seleccion){
            case 1:

                flecha_selector->setPosition(engine->getBaseResolution().x/4 + 180,engine->getBaseResolution().y/8+10);                     
            
            break;

            case 2:

                flecha_selector->setPosition(engine->getBaseResolution().x/4*1.45 -20,engine->getBaseResolution().y/5 + 330);
            
            break;

            case 3:

                flecha_selector->setPosition(engine->getBaseResolution().x/4*2.1 -20,engine->getBaseResolution().y/5 + 330);

            break;

            case 4:

                flecha_selector->setPosition(engine->getBaseResolution().x/4*2.75 -20 ,engine->getBaseResolution().y/5+ 330);

            break;

            case 5:

                flecha_selector->setPosition(engine->getBaseResolution().x/4*1.77 -20 ,engine->getBaseResolution().y/5+ 630);

            break;

            case 6:

                flecha_selector->setPosition(engine->getBaseResolution().x/4*2.42 -20 ,engine->getBaseResolution().y/5+ 630);

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

    coin_purse->setPosition(engine->getBaseResolution().x/8 *5.5,engine->getBaseResolution().y/8);
    coin_purse->setScale(0.2,0.2);
    engine->renderDrawable(coin_purse);

    texto->setString(" " + std::to_string(i_money));
    texto->setPosition(engine->getBaseResolution().x/8 * 6 + 100,engine->getBaseResolution().y/8 + -20);
    texto->setCharacterSize(72);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

}

void ShopState::drawParchment(){

    Engine* engine = Engine::getInstance();
    
    parchment->setPosition(engine->getBaseResolution().x/2+200,engine->getBaseResolution().y/2);
    parchment->setScale(1.5,1.2);
    engine->renderDrawable(parchment);

    npc_cleric->setPosition(engine->getBaseResolution().x/10+60,engine->getBaseResolution().y/10*8-50);
    npc_cleric->setScale(2,2);
    engine->renderDrawable(npc_cleric);

    text_box->setPosition(engine->getBaseResolution().x/10+70,engine->getBaseResolution().y/10*3-50);
    text_box->setScale(1,2);
    engine->renderDrawable(text_box);

    heart_upgrade->setPosition(engine->getBaseResolution().x/4*1.75,engine->getBaseResolution().y/5 + 210);
    heart_upgrade->setScale(0.15,0.15);
    engine->renderDrawable(heart_upgrade);

    if(i_life<maxLvImprovement)
        texto->setString("Lv " + std::to_string(i_life+1));
    else texto->setString("Lv Max ");
    texto->setPosition(engine->getBaseResolution().x/4*1.75,engine->getBaseResolution().y/5 + 310);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    sword_upgrade->setPosition(engine->getBaseResolution().x/4*2.4,engine->getBaseResolution().y/5 + 200);
    sword_upgrade->setScale(0.2,0.2);
    engine->renderDrawable(sword_upgrade);


    if(i_damage<maxLvImprovement)
        texto->setString("Lv " + std::to_string(i_damage+1));
    else texto->setString("Lv Max ");
    texto->setPosition(engine->getBaseResolution().x/4*2.4,engine->getBaseResolution().y/5 + 310);
    texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    if(i_shield != -1){

        shield_upgrade->setPosition(engine->getBaseResolution().x/4*3.05,engine->getBaseResolution().y/5 + 200);
        shield_upgrade->setScale(0.5,0.5);
        engine->renderDrawable(shield_upgrade);
    
        if(i_shield<0)
            texto->setString("Lv " + std::to_string(i_shield+1));
        else texto->setString("Lv Max ");
        texto->setPosition(engine->getBaseResolution().x/4*3.05,engine->getBaseResolution().y/5 + 310);
        texto->setCharacterSize(54);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }

    if((i_hammer != -1)){

        hammer_upgrade->setPosition(engine->getBaseResolution().x/4*2.07,engine->getBaseResolution().y/5 + 500);
        hammer_upgrade->setScale(0.5,0.5);
        engine->renderDrawable(hammer_upgrade);
    
        if(i_hammer<maxLvImprovement)
            texto->setString("Lv " + std::to_string(i_hammer+1));
        else texto->setString("Lv Max ");
        texto->setPosition(engine->getBaseResolution().x/4*2.07,engine->getBaseResolution().y/5 + 610);
        texto->setCharacterSize(54);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }

    if((i_bow != -1)){

        bow_upgrade->setPosition(engine->getBaseResolution().x/4*2.72,engine->getBaseResolution().y/5 + 500);
        bow_upgrade->setScale(0.5,0.5);
        engine->renderDrawable(bow_upgrade);

    
        if(i_bow<maxLvImprovement)
            texto->setString("Lv " + std::to_string(i_bow+1));
        else texto->setString("Lv Max ");
        texto->setPosition(engine->getBaseResolution().x/4*2.72,engine->getBaseResolution().y/5 + 610);
        texto->setCharacterSize(54);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }

}

void ShopState:: drawPrices(){

    Engine* engine = Engine::getInstance();

    if(i_life<maxLvImprovement){

        coin_purse->setPosition(engine->getBaseResolution().x/4*1.75-60,engine->getBaseResolution().y/5 + 378);
        coin_purse->setScale(0.1,0.1);
        engine->renderDrawable(coin_purse);

        texto->setString(std::to_string(basePriceLife*(i_life+1)));
        texto->setPosition(engine->getBaseResolution().x/4*1.75 +20,engine->getBaseResolution().y/5 + 360);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }

    if(i_damage<maxLvImprovement){

        coin_purse->setPosition(engine->getBaseResolution().x/4*2.4-60,engine->getBaseResolution().y/5 + 378);
        coin_purse->setScale(0.1,0.1);
        engine->renderDrawable(coin_purse);

        texto->setString(std::to_string(basePriceDmg*(i_damage+1)));
        texto->setPosition(engine->getBaseResolution().x/4*2.4 +20,engine->getBaseResolution().y/5 + 360);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }
     if(i_shield != -1){

        if(i_shield<0){

            coin_purse->setPosition(engine->getBaseResolution().x/4*3.05-60,engine->getBaseResolution().y/5 + 378);
            coin_purse->setScale(0.1,0.1);
            engine->renderDrawable(coin_purse);

            texto->setString(std::to_string(basePriceArmor*(i_shield+1)));
            texto->setPosition(engine->getBaseResolution().x/4*3.05 +20,engine->getBaseResolution().y/5 + 360);
            texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
            engine->renderDrawable(texto);
        }
    }

    if((i_hammer != -1)){

        if(i_hammer<maxLvImprovement){

            coin_purse->setPosition(engine->getBaseResolution().x/4*2.07-60,engine->getBaseResolution().y/5 + 678);
            coin_purse->setScale(0.1,0.1);
            engine->renderDrawable(coin_purse);

            texto->setString(std::to_string(basePriceArmor*(i_hammer+1)));
            texto->setPosition(engine->getBaseResolution().x/4*2.07 +20,engine->getBaseResolution().y/5 + 660);
            texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
            engine->renderDrawable(texto);
        }
    }

    if((i_bow !=  -1)){

        if(i_bow<maxLvImprovement){

            coin_purse->setPosition(engine->getBaseResolution().x/4*2.72-60,engine->getBaseResolution().y/5 + 678);
            coin_purse->setScale(0.1,0.1);
            engine->renderDrawable(coin_purse);

            texto->setString(std::to_string(basePriceArmor*(i_bow+1)));
            texto->setPosition(engine->getBaseResolution().x/4*2.72 +20,engine->getBaseResolution().y/5 + 660);
            texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
            engine->renderDrawable(texto);
        }
    }

}
