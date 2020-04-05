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
    this->player->setPosition(
        this->map->getPlayerPosition()
    );
    this->player->linkWorldProjectiles(this->projectiles);

    this->initObjectData();
    this->initViewLimits();
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
        enemy->setPosition(data.positon);
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

void Level::adjustPlayerView()
{
    // Read player position and clamp it if necessary
    sf::Vector2f playerPosition = this->player->getPosition();
    if(playerPosition.x < limitLeftUp.x)            playerPosition.x = limitLeftUp.x;
    else if(playerPosition.x > limitRightDown.x)    playerPosition.x = limitRightDown.x;
    if(playerPosition.y < limitLeftUp.y)            playerPosition.y = limitLeftUp.y;
    else if(playerPosition.y > limitRightDown.y)    playerPosition.y = limitRightDown.y;

    Engine::getInstance()->setViewCenter(playerPosition);
}

// Checks
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

void Level::update()
{
    // Entity updates
    for(auto projectile : projectiles)
        projectile->update();
    this->player->update();
    for(auto enemy: enemies)
        enemy->update();

    // Lever testing
    if(Engine::getInstance()->getKeyPressed(sf::Keyboard::Z))
        for(auto lever : levers)
        {
            sf::Vector2f diff = lever->getPosition() - this->player->getPosition();
            float distance = sqrt(diff.x*diff.x + diff.y*diff.y);
            if(distance < lever->getSize().x)
                lever->interact();
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
    if(Engine::getInstance()->getKeyPressed(sf::Keyboard::Return)){
        for(unsigned int i = 0; i< npcs.size(); i++){
            if((this->npcs[i]->getPosition().x < (this->player->getPosition().x + 100)) && (this->npcs[i]->getPosition().x > (this->player->getPosition().x - 100))){
                if(this->npcs[i]->getImShop() == true)
                    this->nextState = SHOP_STATE;
                else{
                    this->player->setNear(npcs[i]->getDialogue());
                    this->nextState = TEXT_STATE;
                }
            }
        }
    }

    // Check deaths
    this->checkEnemyDeaths();
    this->checkDestroyedBullets();

    // Check level exit reached
    this->checkLevelExitReached();

    // Adjust view
    this->adjustPlayerView();
}

void Level::render()
{
    Engine::getInstance()->setFollowView(true);
    this->map->render();

    for(auto lever: levers)
        lever->render();
        
    for(auto npc: npcs)
        npc->render();

    for(auto projectile : projectiles)
        projectile->render();

    for(auto enemy: enemies)
        enemy->render();

    for(auto coin : coins)
        coin->render();
    for(auto tool : tools)
        tool->render();

    this->player->render();

    for(auto door: doors)
        door->render();

    for(auto exit: exits)
        exit->render();
}
