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
    bool b_playerHasLeft;
    int i_exitIndex;
    bool b_hasAnimationBeforeNextLevel;
    int i_bossKeyIndex;

    StateType nextState;

    std::string s_levelName;
    std::string s_zone;

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

    //Variables for bubble to interact
    sf::RectangleShape keyToPress;
    sf::RectangleShape infoBox;

    //Coins picked and not saved
    int pickedCoins;

public:
    Level(Player* player, std::string mapName, const int& entranceIndex);
    ~Level();

    void initObjectData();

    // View related
    void initViewLimits();
    void adjustPlayerView(float frameProgress);

    // Event checks
    void checkLevelExitReached();
    void checkEnemyDeaths();
    void checkDestroyedBullets();

    // Map data related
    std::string getLevelName();
    std::string getLevelZone();

    // Level exit related
    bool didPlayerLeave();
    LevelExit* getActiveExit();
    // Returns if next level must have a animation before it changes
    // Also it resets this value to false to avoid the need to clean it manually
    bool getIfAnimationBeforeNextLevel();
    // Returns a key index if the player reached a boss key. If they didn't reach a key, this will return -1
    int getBossKeyIndex();
    // Sets the reached exit index and sets up everything for GameState to finish the level
    void exitLevel(int exitIndex);
    void saveLevelData();
    bool didPlayerDie();

    // State change
    StateType getNextState();
    void resetNextState();
    void forceInterpolationUpdate();

    //NPCs interaction 
    bool NPCisNear(NPC* npc);
    void renderDialogueBubble(NPC* npc);
    

    void entityUpdate();
    void update();
    void render(float frameProgress);
};

#endif
