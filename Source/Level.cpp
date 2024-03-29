#include "Level.h"

//Point constants multipliers
#define CoinPointMultiplyer 500         //Defienes points when picking coins
#define EnemiesPointMultiplyer 100      // Defines points when killing enemies

Level::Level(Player* player, std::string mapName, const int& entranceIndex)
{
    this->s_levelName = mapName;

    this->b_playerHasLeft = false;
    this->b_hasAnimationBeforeNextLevel = false;
    this->resetNextState();

    sf::Vector2i tileSize = {32,32};
    this->map = new Map(mapName, tileSize, entranceIndex);
    this->s_zone = map->getMetadataValue("zona");

    // Final room check
    std::string final = map->getMetadataValue("esFinal");
    this->b_isFinalBossRoom = (final != "");

    // Player init
    this->player = player;
    this->player->setSize(sf::Vector2f(tileSize.x*3,tileSize.y*3));
    this->player->initPosition(
        this->map->getPlayerPosition()
    );
    this->player->linkWorldProjectiles(this->projectiles);

    this->initObjectData();
    this->initViewLimits();

    //Dialoge box
    keyToPress.setTexture(ResourceManager::getInstance()->loadTexture("resources/enter-key.png"));
    infoBox.setTexture(ResourceManager::getInstance()->loadTexture("resources/text_box.png"));

    //For coins picked but not saved
    pickedCoins = 0;
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
        float statsScaleFactor = 1.0f;
        if(this->s_zone == "cementerio") statsScaleFactor *= 2.0f;
        if(this->s_zone == "torre") statsScaleFactor *= 3.0f;

        Enemy* enemy = EnemyFactory::makeEnemy(this->player, data.size, data.type, statsScaleFactor);
        enemy->initPosition(data.positon);
        enemy->linkWorldProjectiles(this->projectiles);
        this->enemies.push_back(enemy);
    }

    // NPC init
    objectData = this->map->getNPCData();
    for(auto data : objectData)
    {
        NPC* npc = new NPC(data.name);

        if(npc->getImKey() && this->player->getKeyUnlocked(npc->getKeyType())=="1")
        {
            // Change sprite to sleepy Key
            npc->setNewTexture("bosskeySleepyNap.png");
            npc->setOpacity(0.5f);
        }

        // Final boss door is only active when we have the 3 boss keys
        if(npc->getImFinalDoor())
        {
            bool enabled = true;
            for(int i=0; i<3; i++)
                if(player->getKeyUnlocked(i)=="0")
                    enabled = false;

            npc->setInteractable(enabled);
        }

        npc->setSize(data.size);
        npc->setPosition(data.positon);
        this->npcs.push_back(npc);
    }

    // Doors init
    objectData = this->map->getDoorData();
    for(auto data : objectData)
    {
        bool isHorizontal = data.type > 0;
        Door* door = new Door(isHorizontal);
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

        bool active = ftl::GetLeverState(this->s_levelName, counter);
        if(active) lever->interact(false);

        this->levers.push_back(lever);
        counter = this->levers.size();
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

    // Coin init
    counter = 0;
    objectData = this->map->getCoinData();
    for(auto data : objectData)
    {
        bool picked = ftl::GetCoinState(this->s_levelName, counter);
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
}

void Level::initViewLimits()
{
    sf::Vector2f mapSize = this->map->getMapTotalPixelSize();
    sf::Vector2u windowSize = Engine::getInstance()->getReferenceResolution();

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

void Level::checkLevelExitReached()
{
    for(unsigned int i=0; i<exits.size(); i++)
    {
        if(exits[i]->checkPlayerCollision(this->player))
        {
            this->exitLevel(i);
        }
    }
}

bool Level::checkEnemyDeaths()
{
    int killed = 0;
    bool b_screened = false;    //to check if enemy is in the screen
    //get the view, then get it's size and center, then calculate a decent floatRect
    sf::View screen_view=Engine::getInstance()->getView();
    sf::Vector2i viewCenter(screen_view.getCenter());
    sf::Vector2i viewSize(screen_view.getSize());
    sf::FloatRect view_rect(viewCenter.x - viewSize.x / 2,viewCenter.y - viewSize.y / 2,viewSize.x*0.75f,viewSize.y*0.75f);

    for(auto iter = enemies.begin() ; iter != enemies.end() ; ++iter)
    {
        auto position = iter - enemies.begin();

        if(!b_screened && 
            enemies[position]->getBounds().intersects(view_rect)
            && (enemies[position]->getEnemyType() == Enemy::EnemyType::BasicMelee
            || enemies[position]->getEnemyType() == Enemy::EnemyType::BasicRanged)
            && enemies[position]->getType() != 6)   //this last one is temp
        {
            b_screened = true;
        }

        if(enemies[position]->isDead())
        {
            player->addPoints(enemies[position]->getType()* EnemiesPointMultiplyer);
            delete enemies[position];
            enemies.erase(iter);
            killed++;
            --iter;
        }
    }
    this->player->addKill(killed);
    return b_screened;
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

bool Level::checkRealEnemies()
{
    bool thereAre = false;
    for(unsigned int i=0; i<enemies.size() && !thereAre; i++)
    {
        if((enemies[i]->getEnemyType() == Enemy::EnemyType::BasicMelee
            || enemies[i]->getEnemyType() == Enemy::EnemyType::BasicRanged)
            && enemies[i]->getType() != 6)   //this last one is temp
        {
            thereAre = true;
        }
    }
    return thereAre;
}

std::string Level::getLevelName()
{
    return s_levelName;
}

std::string Level::getLevelZone()
{
    return s_zone;
}

bool Level::gameHasBeenBeaten()
{
    return this->b_isFinalBossRoom && !this->checkRealEnemies();
}

// Level exit related
bool Level::didPlayerLeave()
{
    return this->b_playerHasLeft;
}

LevelExit* Level::getActiveExit()
{
    return this->exits[this->i_exitIndex];
}

bool Level::getIfAnimationBeforeNextLevel()
{
    bool result = this->b_hasAnimationBeforeNextLevel;
    this->b_hasAnimationBeforeNextLevel = false;
    return result;
}

void Level::exitLevel(int exitIndex)
{
    this->b_playerHasLeft = true;
    this->pickedCoins = 0;
    this->i_exitIndex = exitIndex;
    this->forceInterpolationUpdate();
}

void Level::saveLevelData()
{
    // Save lever and coin data
    for(unsigned int i=0; i<levers.size(); i++)
        ftl::SetLeverState(s_levelName, i, levers[i]->getIsActive());
    
    for(unsigned int i=0; i<coins.size(); i++)
        ftl::SetCoinState(s_levelName, i, coins[i]->getIsPicked());
}

bool Level::didPlayerDie()
{
    bool dead = this->player->isDead();
    if(dead){
       
        this->player->revive();
        this->pickedCoins = 0;
    }
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

void Level::entityUpdate()
{
    this->player->update();
    for(auto enemy: enemies)
        enemy->update();
}

void Level::update()
{
    // Entity updates
    for(auto projectile : projectiles)
        projectile->update();
    this->entityUpdate();

    // Lever testing
    if(Engine::getInstance()->getKeyPressed(sf::Keyboard::Return))
        for(auto lever : levers)
        {
            sf::Vector2f diff = lever->getPosition() - this->player->getPosition();
            float distance = sqrt(diff.x*diff.x + diff.y*diff.y);
            if(distance < lever->getSize().x && lever->getToggleTime()>0.5f)
            {
                lever->interact();
                lever->restartToggleTime();
            }
        }

    //If the lever is hit by a player attack it toggles (and if some time after last toogle has passed)
    std::vector<Hitbox *> cajas = *Hitbox::getAllHitboxes();
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
    // Coins
    for(auto coin : coins) {
        if(!coin->getIsPicked() && coin->isWithinReach(this->player->getPosition()))
        {
            coin->setIsPicked(true);
            this->player->pickCoin(1);
            this->player->addPoints(CoinPointMultiplyer); //You get points from coins
            pickedCoins++; //To substract points if you die without saving the coins
        }
    }
    // Tools
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
            if(NPCisNear(npcs[i]))
            {
                // NPC can be interacted with
                if(this->npcs[i]->isInteractable())
                {
                    // The final room's door NPC
                    if(this->npcs[i]->getImFinalDoor())
                    {
                        // Prepare the exit to go to the boss' room
                        LevelExit* exit = new LevelExit(FINAL_ROOM_NAME, FINAL_ROOM_DOOR);
                        exit->setSize(sf::Vector2f(0.0f,0.0f));
                        this->exits.push_back(exit);
                        this->exitLevel(this->exits.size()-1);
                    }
                    // A Boss Key "NPC"
                    else if(this->npcs[i]->getImKey())
                    {
                        // Assign the boss key
                        int bossKey = this->npcs[i]->getKeyType();
                        this->player->setRecoveredKey(bossKey);
                        this->player->setAnimationFilename("recoveredKey");
                        this->player->setKeyUnlocked(true, bossKey);

                        // Mark that an animation should play
                        this->b_hasAnimationBeforeNextLevel = true;

                        // Prepare the exit to return to the lobby
                        LevelExit* exit = new LevelExit(LOBBY_NAME, LOBBY_DOOR);
                        exit->setSize(sf::Vector2f(0.0f,0.0f));
                        this->exits.push_back(exit);

                        // Level finished
                        this->exitLevel(this->exits.size()-1);
                    }
                    // A Shop NPC
                    else if(this->npcs[i]->getImShop())
                    {
                        this->nextState = SHOP_STATE;
                    }
                    // A regular talking/note NPC
                    else
                    {
                        this->player->setNear(npcs[i]->getDialogue());
                        this->nextState = TEXT_STATE;
                    }
                    this->forceInterpolationUpdate();
                }
            }
        }
    }

    // Check deaths
    if(this->checkEnemyDeaths())
        //music must go action
        ResourceManager::getInstance()->MusicToAction();
    else
        //music must calm down
        if(!this->checkRealEnemies())
        ResourceManager::getInstance()->MusicToMellow();
    
    this->checkDestroyedBullets();

    // Check level exit reached
    this->checkLevelExitReached();
}

void Level::render(float frameProgress)
{
    // Adjust view
    this->adjustPlayerView(frameProgress);

    Engine::getInstance()->setFollowView(true);

    this->map->renderBackground(this->player->getInterpolatedPosition(frameProgress));
    this->map->renderBack();

    for(auto lever: levers)
        lever->render();
        
    for(auto npc: npcs){
        npc->render();
        if(NPCisNear(npc))
        {
            if(npc->isInteractable())
            {
                // Set the interact UI button to grey, so it says "Nope you can't interact with me"
                this->keyToPress.setFillColor(sf::Color(255,255,255,255));
                this->infoBox.setFillColor(sf::Color(255,255,255,255));
            }
            else
            {
                // Set the interact UI button to grey, so it says "Nope you can't interact with me"
                this->keyToPress.setFillColor(sf::Color(100,100,100,255));
                this->infoBox.setFillColor(sf::Color(100,100,100,255));
            }
            //If there is an NPC near we render the dialogue bubble
            renderDialogueBubble(npc);    
        }
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

    this->map->renderFront();
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
