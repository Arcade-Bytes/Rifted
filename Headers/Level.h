#ifndef LEVEL_H
#define LEVEL_H

#include "Engine.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Lever.h"
#include "Door.h"
#include "BreakableDoor.h"
#include "LevelExit.h"

class Level {
private:
    bool b_playerLeaves;
    int i_exitIndex;

    Player* player;
    std::vector<Enemy*> enemies;
    std::vector<Lever*> levers;
    std::vector<Door*> doors;
    std::vector<LevelExit*> exits;

    Map* map;
public:
    Level(Player* player, std::string mapName, const int& entranceIndex);
    ~Level();

    bool didPlayerLeave();
    LevelExit* getActiveExit();

    void update();
    void render();
};

#endif