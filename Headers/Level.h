#ifndef LEVEL_H
#define LEVEL_H

#include "Engine.h"
#include "State.h"
#include "FileManager.h"
#include "Map.h"
#include "NPC.h"
#include "Pickable.h"
#include "Player.h"
#include "EnemyFactory.h"
#include "Lever.h"
#include "Door.h"
#include "BreakableDoor.h"
#include "LevelExit.h"

class Level {
private:
    bool b_playerLeaves;
    int i_exitIndex;

    StateType nextState;

    std::string levelName;

    // View properties
    sf::Vector2i limitLeftUp;
    sf::Vector2i limitRightDown;

    Player* player;
    std::vector<Enemy*> enemies;
    std::vector<NPC*> npcs;
    std::vector<Projectile*> projectiles;
    std::vector<Lever*> levers;
    std::vector<Door*> doors;
    std::vector<Pickable*> coins;
    std::vector<Pickable*> tools;
    std::vector<LevelExit*> exits;

    Map* map;
public:
    Level(Player* player, std::string mapName, const int& entranceIndex);
    ~Level();

    void initObjectData();

    // View related
    void initViewLimits();
    void adjustPlayerView(float frameProgress);

    // Event checks
    void checkLevelExitReached();
    bool checkEnemyDeaths();
    void checkDestroyedBullets();

    // Level exit related
    bool didPlayerLeave();
    LevelExit* getActiveExit();
    void saveLevelData();
    bool didPlayerDie();

    // State change
    StateType getNextState();
    void resetNextState();
    void forceInterpolationUpdate();

    void update();
    void render(float frameProgress);
};

#endif
