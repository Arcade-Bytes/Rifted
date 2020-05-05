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

#define LOBBY_NAME "Hub_World"
#define LOBBY_DOOR 4
#define FINAL_ROOM_NAME "Prueba_3"
#define FINAL_ROOM_DOOR 1

class Level {
private:
    bool b_playerHasLeft;
    int i_exitIndex;
    bool b_hasAnimationBeforeNextLevel;
    int i_bossKeyIndex;

    StateType nextState;

    std::string s_levelName;
    std::string s_zone;
    bool b_isFinalBossRoom;

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

    // Calculate view limits to limit the view movement later
    void initViewLimits();
    //Adjust player view depending on player position
    void adjustPlayerView(float frameProgress);

    // Event checks
    //Checks if player collides to level exit to change map
    void checkLevelExitReached();
    //Deletes enemies marked as dead
    bool checkEnemyDeaths();
    //Deletes the bullets tagged as destroyed
    void checkDestroyedBullets();
    // Returns true if there are REAL enemies in the list (BasicRanged and BasicMelee)
    bool checkRealEnemies();

    // Returns the level name. Level name is the .tmx map name, without extension
    // (example: Test.tmx defines a level named Test)
    std::string getLevelName();
    // Return the level zone, specified in the map's metadata. Zones are "tutorial","mina","cementerio" or "tower"
    std::string getLevelZone();
    // Returns true if this level is the final boss' room, as specified in the map's metadata
    // and the level has zero enemies, thus beating the game
    bool gameHasBeenBeaten();

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
