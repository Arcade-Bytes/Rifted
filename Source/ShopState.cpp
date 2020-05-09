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
    arrowMove = prevArrowMove = 0;
    max_seleccion = 2;//Vida y money nunca seran -1

    loadAssets();
    loadDialogue();
}

ShopState::~ShopState()
{
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
    pointer.setCharacterSize(Engine::getInstance()->getBaseResolution().x*0.018229f);
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
    Engine* engine = Engine::getInstance();
    ResourceManager* resources = ResourceManager::getInstance();
    sf::Vector2f baseRes = sf::Vector2f(engine->getBaseResolution().x,engine->getBaseResolution().y);

    //Cargamos la fuente de texto
    texto= new sf::Text();
    texto->setFont(*resources->loadFont("resources/XOX.ttf"));
    texto->setColor(sf::Color::White);
    texto_render.setFont(*resources->loadFont("resources/XOX.ttf"));
    pointer.setFont(*resources->loadFont("resources/XOX.ttf"));

    //Cargamos la flecha
    flecha_selector = new sf::RectangleShape();
    flecha_selector->setTexture(resources->loadTexture("resources/black_arrow.png"));
    flecha_selector->rotate(180);
    flecha_selector->setSize(sf::Vector2f(baseRes.x*0.041666f,baseRes.y*0.074f)); // 80x80 sobre 1920x1080
    flecha_selector->setOrigin(flecha_selector->getSize()/2.0f);

    //Cargamos la flecha
    parchment = new sf::RectangleShape();
    parchment->setTexture(resources->loadTexture("resources/parchment.png"));
    parchment->rotate(90);
    parchment->setSize(sf::Vector2f(baseRes.x*0.9f,baseRes.y*1.53f));
    parchment->setPosition(baseRes.x*1.05f,-baseRes.y*0.34f);

    //Cargamos las mejoras de vida
    heart_upgrade = new sf::RectangleShape();
    heart_upgrade->setTexture(resources->loadTexture("resources/health_upgrade.png"));
    heart_upgrade->setSize(sf::Vector2f(baseRes.x*0.09375f,baseRes.y*0.1666666f)); // 180x180 sobre 1920x1080
    heart_upgrade->setOrigin(heart_upgrade->getSize()/2.0f);
    heart_upgrade->setPosition(baseRes.x/4*1.75,baseRes.y/5 + baseRes.y*0.185f);

    //Cargamos las mejoras de espada
    sword_upgrade = new sf::RectangleShape();
    sword_upgrade->setTexture(resources->loadTexture("resources/sword_upgrade.png"));
    sword_upgrade->setSize(sf::Vector2f(baseRes.x*0.09375f,baseRes.y*0.1666666f)); // 180x180 sobre 1920x1080
    sword_upgrade->setOrigin(sword_upgrade->getSize()/2.0f);
    sword_upgrade->setPosition(baseRes.x/4*2.4,baseRes.y/5 + baseRes.y*0.185f);
    //sword_upgrade->setScale(0.2,0.2);

    //Cargamos las monedas
    coin_purse = new sf::RectangleShape();
    coin_purse->setTexture(resources->loadTexture("resources/coin.png"));

    //Cargamos el escudo
    shield_upgrade = new sf::RectangleShape();
    shield_upgrade->setTexture(resources->loadTexture("resources/shield.png"));
    shield_upgrade->setSize(sf::Vector2f(baseRes.x*0.09375f,baseRes.y*0.1666666f)); // 180x180 sobre 1920x1080
    shield_upgrade->setOrigin(shield_upgrade->getSize()/2.0f);
    shield_upgrade->setPosition(baseRes.x/4*3.05,baseRes.y/5 + baseRes.y*0.185f);
    //shield_upgrade->setScale(0.5,0.5);

    //Cargamos el martillo
    hammer_upgrade = new sf::RectangleShape();
    hammer_upgrade->setTexture(resources->loadTexture("resources/hammer.png"));
    hammer_upgrade->setSize(sf::Vector2f(baseRes.x*0.09375f,baseRes.y*0.1666666f)); // 180x180 sobre 1920x1080
    hammer_upgrade->setOrigin(hammer_upgrade->getSize()/2.0f);
    hammer_upgrade->setPosition(baseRes.x/4*2.07,baseRes.y/5 + baseRes.y*0.46296f);
    //hammer_upgrade->setScale(0.5,0.5);

    //Cargamos el arco
    bow_upgrade = new sf::RectangleShape();
    bow_upgrade->setTexture(resources->loadTexture("resources/bow.png"));
    bow_upgrade->setSize(sf::Vector2f(baseRes.x*0.09375f,baseRes.y*0.1666666f)); // 180x180 sobre 1920x1080
    bow_upgrade->setOrigin(bow_upgrade->getSize()/2.0f);
    bow_upgrade->setPosition(baseRes.x/4*2.72,baseRes.y/5 + baseRes.y*0.46296f);
    //bow_upgrade->setScale(0.5,0.5);

    //Cargamos al tendero
    npc_cleric = new sf::RectangleShape();
    npc_cleric->setTexture(resources->loadTexture("resources/cleric.png"));
    npc_cleric->setSize(sf::Vector2f(baseRes.x*0.28f,baseRes.y*0.537f));
    npc_cleric->setPosition(baseRes.x*0.f,baseRes.y*0.45f);

    //Cargamos la caja de texto
    text_box = new sf::RectangleShape();
    text_box->setTexture(resources->loadTexture("resources/text_box.png"));
    text_box->setSize(sf::Vector2f(baseRes.x*0.27f,baseRes.y*0.37f));
    text_box->setPosition(baseRes.x*0.f,baseRes.y*0.05f);
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

    if(engine->getKeyPressed(sf::Keyboard::Return))
    {
        switch(seleccion){
            case 1: //SI LE DA AL BOTON DE SALIR ....
                texto_render = textoTienda_v5[random()%textoTienda_v5.size()]; //CARGAMOS UNA DESPEDIDA
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
    else if(engine->getKeyPressed(sf::Keyboard::Escape))
    {
        this->changeState(GAME_STATE, false);
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

void ShopState:: render(float frameProgress){
    Engine::getInstance()->setFollowView(false);
    drawParchment();
    drawPlayerData();
    drawText();
    drawArrow(frameProgress);
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
    texto_render.setPosition(engine->getBaseResolution().x*0.1406f,engine->getBaseResolution().y/4);
    texto_render.setOrigin(texto_render.getLocalBounds().width/2.0f,texto_render.getLocalBounds().height/2.0f);
    engine->renderDrawable(&texto_render);

    //Boton atras
    texto->setString("Exit");
    texto->setPosition(engine->getBaseResolution().x*0.4375f,engine->getBaseResolution().y/8);
    texto->setCharacterSize(engine->getBaseResolution().x*0.028125f);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

}

void ShopState:: drawArrow(float frameProgress)
{
    Engine* engine = Engine::getInstance();
    float separation = engine->getBaseResolution().x * 0.1625; // 312 sobre 1920
    switch(seleccion){
        case 1:
            flecha_selector->setPosition(engine->getBaseResolution().x*0.34375f,                engine->getBaseResolution().y*0.134f);   // 660, 145       
            break;
        case 2:
            flecha_selector->setPosition(engine->getBaseResolution().x*0.352f,                  engine->getBaseResolution().y*0.50555f); // 676, 546
            break;
        case 3:
            flecha_selector->setPosition(engine->getBaseResolution().x*0.352f + separation,     engine->getBaseResolution().y*0.50555f); // 988. 546
            break;
        case 4:
            flecha_selector->setPosition(engine->getBaseResolution().x*0.352f + separation*2,   engine->getBaseResolution().y*0.50555f);  // 1300, 546
            break;
        case 5:
            flecha_selector->setPosition(engine->getBaseResolution().x*0.432f,                  engine->getBaseResolution().y*0.78333f);  // 830, 846
            break;
        case 6:
            flecha_selector->setPosition(engine->getBaseResolution().x*0.432f + separation,     engine->getBaseResolution().y*0.78333f);  // 1142, 846
            break;
    }

    float finalMove = this->prevArrowMove + (this->arrowMove - this->prevArrowMove) * frameProgress;
    finalMove *= engine->getBaseResolution().x * 0.015625f;
    flecha_selector->move(finalMove,0);
    engine->renderDrawable(flecha_selector);

}

void ShopState:: drawPlayerData(){

    Engine* engine = Engine::getInstance();
    sf::Vector2f baseRes = sf::Vector2f(engine->getBaseResolution().x,engine->getBaseResolution().y);

    texto->setColor(sf::Color::Black);

    coin_purse->setSize(sf::Vector2f(baseRes.x*0.064375f,baseRes.y*0.114444444444f));
    coin_purse->setOrigin(coin_purse->getSize()/2.0f);
    coin_purse->setPosition(baseRes.x/8 *5.5,baseRes.y/8);
    engine->renderDrawable(coin_purse);

    texto->setString(" " + std::to_string(i_money));
    texto->setPosition(baseRes.x*0.802f,baseRes.y*0.1065f);
    texto->setCharacterSize(baseRes.x*0.0375);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

}

void ShopState::drawParchment(){

    Engine* engine = Engine::getInstance();
    sf::Vector2f baseRes = sf::Vector2f(engine->getBaseResolution().x,engine->getBaseResolution().y);
    
    engine->renderDrawable(parchment);
    engine->renderDrawable(npc_cleric);
    engine->renderDrawable(text_box);

    engine->renderDrawable(heart_upgrade);

    if(i_life<maxLvImprovement)
        texto->setString("Lv " + std::to_string(i_life+1));
    else texto->setString("Lv Max ");
    texto->setPosition(baseRes.x/4*1.75,baseRes.y/5 + baseRes.y*0.287f);
    texto->setCharacterSize(baseRes.x*0.028125f);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    engine->renderDrawable(sword_upgrade);


    if(i_damage<maxLvImprovement)
        texto->setString("Lv " + std::to_string(i_damage+1));
    else texto->setString("Lv Max ");
    texto->setPosition(baseRes.x/4*2.4,baseRes.y/5 + baseRes.y*0.287f);
    //texto->setCharacterSize(54);
    texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
    engine->renderDrawable(texto);

    if(i_shield != -1){
        engine->renderDrawable(shield_upgrade);
    
        if(i_shield<0)
            texto->setString("Lv " + std::to_string(i_shield+1));
        else texto->setString("Lv Max ");
        texto->setPosition(baseRes.x/4*3.05,baseRes.y/5 + baseRes.y*0.287f);
        //texto->setCharacterSize(54);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }

    if((i_hammer != -1)){
        engine->renderDrawable(hammer_upgrade);
    
        if(i_hammer<maxLvImprovement)
            texto->setString("Lv " + std::to_string(i_hammer+1));
        else texto->setString("Lv Max ");
        texto->setPosition(baseRes.x/4*2.07,baseRes.y/5 + baseRes.y*0.5648f);
        //texto->setCharacterSize(54);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }

    if((i_bow != -1)){
        engine->renderDrawable(bow_upgrade);

    
        if(i_bow<maxLvImprovement)
            texto->setString("Lv " + std::to_string(i_bow+1));
        else texto->setString("Lv Max ");
        texto->setPosition(baseRes.x/4*2.72,baseRes.y/5 + baseRes.y*0.5648f);
        //texto->setCharacterSize(54);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }

}

void ShopState:: drawPrices(){

    Engine* engine = Engine::getInstance();
    sf::Vector2f baseRes = sf::Vector2f(engine->getBaseResolution().x,engine->getBaseResolution().y);

    coin_purse->setSize(sf::Vector2f(baseRes.x*0.0321875f,baseRes.y*0.057222222f)); // 61.8 x 61.8 sobre 1920 x 1080
    coin_purse->setOrigin(coin_purse->getSize()/2.0f);

    if(i_life<maxLvImprovement){

        coin_purse->setPosition(baseRes.x/4*1.75-baseRes.x*0.03125f,baseRes.y/5 + baseRes.y*0.35f);
        engine->renderDrawable(coin_purse);

        texto->setString(std::to_string(basePriceLife*(i_life+1)));
        texto->setPosition(baseRes.x/4*1.75 +baseRes.x*0.01,baseRes.y/5 + baseRes.y*0.3333333f);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }

    if(i_damage<maxLvImprovement){

        coin_purse->setPosition(baseRes.x/4*2.4-baseRes.x*0.03125f,baseRes.y/5 + baseRes.y*0.35f);
        engine->renderDrawable(coin_purse);

        texto->setString(std::to_string(basePriceDmg*(i_damage+1)));
        texto->setPosition(baseRes.x/4*2.4 +baseRes.x*0.01,baseRes.y/5 + baseRes.y*0.3333333f);
        texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
        engine->renderDrawable(texto);
    }
     if(i_shield != -1){

        if(i_shield<0){

            coin_purse->setPosition(baseRes.x/4*3.05-baseRes.x*0.03125f,baseRes.y/5 + baseRes.y*0.35f);
            engine->renderDrawable(coin_purse);

            texto->setString(std::to_string(basePriceArmor*(i_shield+1)));
            texto->setPosition(baseRes.x/4*3.05 +baseRes.x*0.01,baseRes.y/5 + baseRes.y*0.3333333f);
            texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
            engine->renderDrawable(texto);
        }
    }

    if((i_hammer != -1)){

        if(i_hammer<maxLvImprovement){

            coin_purse->setPosition(baseRes.x/4*2.07-baseRes.x*0.03125f,baseRes.y/5 + baseRes.y*0.6277777777f);
            engine->renderDrawable(coin_purse);

            texto->setString(std::to_string(basePriceArmor*(i_hammer+1)));
            texto->setPosition(baseRes.x/4*2.07 +baseRes.x*0.01,baseRes.y/5 + baseRes.y*0.6111111f);
            texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
            engine->renderDrawable(texto);
        }
    }

    if((i_bow !=  -1)){

        if(i_bow<maxLvImprovement){

            coin_purse->setPosition(baseRes.x/4*2.72-baseRes.x*0.03125f,baseRes.y/5 + baseRes.y*0.6277777777f);
            engine->renderDrawable(coin_purse);

            texto->setString(std::to_string(basePriceArmor*(i_bow+1)));
            texto->setPosition(baseRes.x/4*2.72 +baseRes.x*0.01,baseRes.y/5 + baseRes.y*0.6111111f);
            texto->setOrigin(texto->getLocalBounds().width/2.0f,texto->getLocalBounds().height/2.0f);
            engine->renderDrawable(texto);
        }
    }

}
