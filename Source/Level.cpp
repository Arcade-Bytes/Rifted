#include "Level.h"

Level::Level(Player* player, std::string mapName, const int& entranceIndex)
{
    this->levelName = mapName;

    this->b_playerLeaves = false;
    this->resetNextState();

    sf::Vector2i tileSize = {32,32};
    this->map = new Map(mapName, tileSize, entranceIndex);

    // Player init
    this->player = player;
    this->player->setSize(sf::Vector2f(tileSize.x*2,tileSize.y*2));
    this->player->initPosition(
        this->map->getPlayerPosition()
    );
    this->player->linkWorldProjectiles(this->projectiles);

    this->initObjectData();
    this->initViewLimits();

    //Dialoge box
    keyToPress.setTexture(ResourceManager::getInstance()->loadTexture("resources/enter-key.png"));
    infoBox.setTexture(ResourceManager::getInstance()->loadTexture("resources/text_box.png"));
}

Level::~Level()
{
    for(auto enemy : enemies)
    {
        delete enemy;
        enemy = NULL;
    }
    enemies.clear();
    for(auto npc : npcs)
    {
        delete npc;
        npc = NULL;
    }
    npcs.clear();
    for(auto projectile : projectiles)
    {
        delete projectile;
        projectile = NULL;
    }
    projectiles.clear();
    for(auto lever : levers)
    {
        delete lever;
        lever = NULL;
    }
    levers.clear();
    for(auto door : doors)
    {
        delete door;
        door = NULL;
    }
    doors.clear();
    for(auto exit : exits)
    {
        delete exit;
        exit = NULL;
    }
    exits.clear();
    delete this->map;
}

void Level::initObjectData()
{
    // Enemies init
    std::vector<MapObject> objectData = this->map->getEnemyData();
    for(auto data : objectData)
    {
        Enemy* enemy = EnemyFactory::makeEnemy(this->player, data.size, data.type);
        enemy->initPosition(data.positon);
        enemy->linkWorldProjectiles(this->projectiles);
        this->enemies.push_back(enemy);
    }

    // NPC init
    objectData = this->map->getNPCData();
    for(auto data : objectData)
    {
        NPC* npc = new NPC(data.name);
        npc->setSize(data.size);
        npc->setPosition(data.positon);
        this->npcs.push_back(npc);
    }

    // Doors init
    objectData = this->map->getDoorData();
    for(auto data : objectData)
    {
        Door* door = new Door();
        door->setSize(data.size);
        door->setPosition(data.positon);
        door->setVinculationId(data.name);
        this->doors.push_back(door);
    }

    // Levers init
    objectData = this->map->getLeverData();
    int counter = 0;
    for(auto data : objectData)
    {
        Lever* lever = new Lever();
        lever->setSize(data.size);
        lever->setPosition(data.positon);

        // Vinculate to doors
        for(auto door : doors)
            if(door->getVinculationId() == data.name)
                lever->addDoor(door);

        bool active = ftl::GetLeverState(this->levelName, counter);
        if(active) lever->interact();

        this->levers.push_back(lever);
        counter = this->levers.size();
    }

    // Coin init
    counter = 0;
    objectData = this->map->getCoinData();
    for(auto data : objectData)
    {
        bool picked = ftl::GetCoinState(this->levelName, counter);
        Pickable* coin = new Pickable(picked);
        coin->setSize(data.size);
        coin->setPosition(data.positon);
        this->coins.push_back(coin);
        counter = this->coins.size();
    }

    // Tools
    objectData = this->map->getToolData();
    for(auto data : objectData)
    {
        bool unlocked = this->player->getIsWeaponUnlocked(data.name);
        Pickable* tool = new Pickable(unlocked, data.name);
        tool->setSize(data.size);
        tool->setPosition(data.positon);
        this->tools.push_back(tool);
    }

    // Exit init
    objectData = this->map->getExitData();
    for(auto data : objectData)
    {
        int splitter = data.name.find("/");
        std::string mapFile = data.name.substr(0,splitter);
        std::string index = data.name.substr(splitter+1);
        int entrance = atoi(index.c_str());
        
        LevelExit* exit = new LevelExit(mapFile, entrance);
        exit->setSize(data.size);
        exit->setPosition(data.positon);
        this->exits.push_back(exit);
    }
}

// View related
void Level::initViewLimits()
{
    sf::Vector2f mapSize = this->map->getMapTotalPixelSize();
    sf::Vector2u windowSize = Engine::getInstance()->getBaseResolution();

    this->limitLeftUp.x = this->limitRightDown.x = mapSize.x / 2;
    this->limitLeftUp.y = this->limitRightDown.y = mapSize.y / 2;
    if(mapSize.x > windowSize.x)
    {
        limitLeftUp.x       = windowSize.x/2;
        limitRightDown.x    = mapSize.x - windowSize.x/2;
    }
    if(mapSize.y > windowSize.y)
    {
        limitLeftUp.y       = windowSize.y/2;
        limitRightDown.y    = mapSize.y - windowSize.y/2;
    }
}
    //Adjust player view depending on player position
void Level::adjustPlayerView(float frameProgress)
{
    // Read player position and clamp it if necessary
    sf::Vector2f playerPosition = this->player->getInterpolatedPosition(frameProgress);
    if(playerPosition.x < limitLeftUp.x)            playerPosition.x = limitLeftUp.x;
    else if(playerPosition.x > limitRightDown.x)    playerPosition.x = limitRightDown.x;
    if(playerPosition.y < limitLeftUp.y)            playerPosition.y = limitLeftUp.y;
    else if(playerPosition.y > limitRightDown.y)    playerPosition.y = limitRightDown.y;

    Engine::getInstance()->setViewCenter(playerPosition);
}

//Checks if player collides to level exit to change map
void Level::checkLevelExitReached()
{
    for(unsigned int i=0; i<exits.size(); i++)
    {
        if(exits[i]->checkPlayerCollision(this->player))
        {
            this->b_playerLeaves = true;
            this->i_exitIndex = i;
        }
    }
}
//Deletes enemies marked as dead
void Level::checkEnemyDeaths()
{
    int killed = 0;
    for(auto iter = enemies.begin() ; iter != enemies.end() ; ++iter)
    {
        auto position = iter - enemies.begin();

        if(enemies[position]->isDead())
        {
            delete enemies[position];
            enemies.erase(iter);
            killed++;
            --iter;
        }
    }
    this->player->addKill(killed);
}

//Deletes the bullets tagged destroyed
void Level::checkDestroyedBullets()
{
    for(auto iter = projectiles.begin() ; iter != projectiles.end() ; ++iter)
    {
        auto position = iter - projectiles.begin();

        if(projectiles[position]->isDestroyed())
        {
            delete projectiles[position];
            projectiles.erase(iter);
            --iter;
        }
    }
}

// Level exit related
bool Level::didPlayerLeave()
{
    return this->b_playerLeaves;
}

LevelExit* Level::getActiveExit()
{
    return this->exits[this->i_exitIndex];
}

void Level::saveLevelData()
{
    // Save lever and coin data
    for(unsigned int i=0; i<levers.size(); i++)
        ftl::SetLeverState(levelName, i, levers[i]->getIsActive());
    
    for(unsigned int i=0; i<coins.size(); i++)
        ftl::SetCoinState(levelName, i, coins[i]->getIsPicked());
}

bool Level::didPlayerDie()
{
    bool dead = this->player->isDead();
    if(dead) this->player->revive();
    return dead;
}

// State change
StateType Level::getNextState()
{
    return this->nextState;
}

void Level::resetNextState()
{
    this->nextState = GAME_STATE;
}

void Level::forceInterpolationUpdate()
{
    this->player->updateInterpolationPositions();
    for(auto enemy : enemies)
        enemy->updateInterpolationPositions();
    for(auto projectile : projectiles)
        projectile->updateInterpolationPositions();
}

void Level::update()
{
    // Entity updates
    for(auto projectile : projectiles)
        projectile->update();
    this->player->update();
    for(auto enemy: enemies)
        enemy->update();
    std::vector<Hitbox *> cajas = *Hitbox::getAllHitboxes();
    // Lever testing
    if(Engine::getInstance()->getKeyPressed(sf::Keyboard::Return))
        for(auto lever : levers)
        {
            sf::Vector2f diff = lever->getPosition() - this->player->getPosition();
            float distance = sqrt(diff.x*diff.x + diff.y*diff.y);
            if(distance < lever->getSize().x)
                lever->interact();
        }

    //If the lever is hit by a player attack it toggles (and if somo time after last toogle has passed)
    for(auto lever: levers){
        for(auto box: cajas){
            if(box->getType() == PLAYER_ATTACK && lever->getToggleTime()>0.5f && box->getSize().x > 0){
                sf::Vector2f diff = lever->getPosition() - box->getPosition();
                float distance = sqrt(diff.x*diff.x + diff.y*diff.y);
                if(distance < lever->getSize().x){
                    lever->interact();
                    lever->restartToggleTime();
                }
            }
        }
    }

    // Interactables
    for(auto coin : coins) {
        if(!coin->getIsPicked() && coin->isWithinReach(this->player->getPosition()))
        {
            coin->setIsPicked(true);
            this->player->pickCoin(1);
        }
    }
    for(auto tool : tools) {
        if(!tool->getIsPicked() && tool->isWithinReach(this->player->getPosition()))
        {
            tool->setIsPicked(true);
            this->player->unlockWeapon(tool->getName());
        }
    }

    //If player is near an NPC, it interacts with it
    if(Engine::getInstance()->getKeyPressed(sf::Keyboard::Return)){
        for(unsigned int i = 0; i< npcs.size(); i++){
            if(NPCisNear(npcs[i])){
                if(this->npcs[i]->getImShop() == true)
                    this->nextState = SHOP_STATE;
                else{
                    this->player->setNear(npcs[i]->getDialogue());
                    this->nextState = TEXT_STATE;
                }
                this->forceInterpolationUpdate();
            }
        }
    }

    // Check deaths
    this->checkEnemyDeaths();
    this->checkDestroyedBullets();

    // Check level exit reached
    this->checkLevelExitReached();
}

void Level::render(float frameProgress)
{
    // Adjust view
    this->adjustPlayerView(frameProgress);

    Engine::getInstance()->setFollowView(true);

    this->map->render();

    for(auto lever: levers)
        lever->render();
        
    for(auto npc: npcs){
        npc->render();
        if(NPCisNear(npc)) //If there is an NPC near we render the dialogue bubble
            renderDialogueBubble(npc);    
        
    }

    for(auto projectile : projectiles)
        projectile->render(frameProgress);

    for(auto enemy: enemies)
        enemy->render(frameProgress);

    for(auto coin : coins)
        coin->render();
    for(auto tool : tools)
        tool->render();

    this->player->render(frameProgress);

    for(auto door: doors)
        door->render();

    for(auto exit: exits)
        exit->render();
}

//Checks if NPC is near player

bool Level::NPCisNear(NPC* npc){

    bool near = false;

    sf::Vector2f diff = npc->getPosition() - this->player->getPosition();
    float distance = sqrt(diff.x*diff.x + diff.y*diff.y);
    if(distance < npc->getSize().x)
        near = true;

    return near;

}

//Renders the dialogue bubble

void Level:: renderDialogueBubble(NPC* npc){

    Engine* engine = Engine::getInstance();

    keyToPress.setSize(sf::Vector2f(1.5 * npc->getSize().x/4, 1.5 * npc->getSize().y/4));
    infoBox.setSize(sf::Vector2f(3 * npc->getSize().x/4, 3 * npc->getSize().y/4));
    keyToPress.setOrigin(keyToPress.getSize().x/2,keyToPress.getSize().y/2);
    keyToPress.setPosition(npc->getPosition().x, npc->getPosition().y - npc->getSize().y);
    infoBox.setOrigin(infoBox.getSize().x/2,infoBox.getSize().y/2);
    infoBox.setPosition(npc->getPosition().x, npc->getPosition().y - npc->getSize().y);
    engine->renderDrawable(&infoBox);
    engine->renderDrawable(&keyToPress);

}
