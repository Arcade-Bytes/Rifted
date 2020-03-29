#include "Level.h"

Level::Level(Player* player, std::string mapName, const int& entranceIndex)
{
    this->b_playerLeaves = false;

    this->map = new Map(mapName, {16,16}, entranceIndex);

    // Player init
    this->player = player;
    this->player->setPosition(
        this->map->getPlayerPosition()
    );

    // Enemies init
    std::vector<MapObject> enemyData = this->map->getEnemyData();
    for(auto data : enemyData)
    {
        Enemy* enemy = new Enemy(this->player);
        enemy->setPosition(data.positon);
        enemy->setSize(data.size);
        this->enemies.push_back(enemy);
    }

    // Doors init
    std::vector<MapObject> doorData = this->map->getDoorData();
    for(auto data : doorData)
    {
        Door* door = new Door();
        door->setPosition(data.positon);
        door->setSize(data.size);
        door->setVinculationId(data.name);
        this->doors.push_back(door);
    }

    // Levers init
    std::vector<MapObject> leverData = this->map->getLeverData();
    for(auto data : leverData)
    {
        Lever* lever = new Lever();
        lever->setPosition(data.positon);
        lever->setSize(data.size);

        // Vinculate to doors
        for(auto door : doors)
            if(door->getVinculationId() == data.name)
                lever->addDoor(door);

        this->levers.push_back(lever);
    }

    // Exit init
    std::vector<MapObject> exitData = this->map->getExitData();
    for(auto data : exitData)
    {
        int splitter = data.name.find("/");
        std::string mapFile = data.name.substr(0,splitter);
        std::string index = data.name.substr(splitter+1);
        int entrance = atoi(index.c_str());
        
        LevelExit* exit = new LevelExit(mapFile, entrance);
        exit->setPosition(data.positon);
        exit->setSize(data.size);
        this->exits.push_back(exit);
    }
}

Level::~Level()
{
    for(auto enemy : enemies)
    {
        delete enemy;
        enemy = NULL;
    }
    enemies.clear();
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

bool Level::didPlayerLeave()
{
    return this->b_playerLeaves;
}

LevelExit* Level::getActiveExit()
{
    return this->exits[this->i_exitIndex];
}

void Level::update()
{
    this->player->update();

    for(auto enemy: enemies)
        enemy->update();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        for(auto lever : levers)
            lever->interact();
    }

    // Check level exit reached
    for(unsigned int i=0; i<exits.size(); i++)
    {
        if(exits[i]->checkPlayerCollision(this->player))
        {
            this->b_playerLeaves = true;
            this->i_exitIndex = i;
        }
    }

    // Adjust view
    Engine::getInstance()->setViewCenter(this->player->getPosition());
}

void Level::render()
{
    this->map->render();
    this->player->render();

    for(auto enemy: enemies)
        enemy->render();

    for(auto door: doors)
        door->render();

    for(auto lever: levers)
        lever->render();

    for(auto exit: exits)
        exit->render();
}
