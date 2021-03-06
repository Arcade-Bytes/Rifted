#ifndef PLAYER_H
#define PLAYER_H

#include "Engine.h"
#include "Entity.h"
#include "Weapon.h"
#include "Shield.h"
#include "NPC.h"

class Player : public Entity {
private:
    // Life regeneration properties
    float f_regenerationDelta;      // Time since last damage/healing action
    float f_regenerationCooldown;   // Necessary time to start regenerating
    float f_regenerationSpeed;      // Time between regen and regen once you're already regenerating
    float f_regenerationAmount;     // Amount of life regenerated every time
    int i_nchunks;                  // Number of chunks the lifebar is divided in

    // Potion properties
    int i_maxPotions;
    int i_potions;

    int i_totalCoins;
    int i_coins;                    // Amount of coins
    int i_deaths;                   // Amount of player deaths... does it even die?
    int i_kills;                    // Amount of kills
    int i_door;                     // Door we used to enter the level
    int i_healthUpg;                // Current health level, multiplier?
    int i_score;                    // Score
    std::string s_levelName;        // Current level name
    bool b_usedPotionLastFrame;

    // Game progress
    bool vb_mainKeys[3];

    // Data transfer between states
    std::vector<std::string> vs_nearDialogue; // The text the NPC we interacted with has to say. Sent from GameState to TextState
    std::string s_animationFile; // The animation file name we must read in the next animation. Sent from GameState to AnimationState
    int i_newKeyIndex; // The index (int) of the new achieved key. Sent from GameState to AnimationState


    Weapon* sword;
    Weapon* hammer;
    RangedWeapon* bow;
    Shield* shield;

public:
    Player(const float& maxHealth);
    ~Player();

    // Reduces damage by shield protection and takes damage. If life poitns are 0 this thing dies
    float getHurt(float& damage);
    // Adds life points to current life, capped to max life
    float getHealed(float& healing);
    // Meant for player's update(). Regenerates life every once in a while
    void regenerate();

    // Returns the (integer) amount of potions the player can carry
    int getMaxPotions();
    // Returns the (integer) amount of potions the player currently has
    int getRemainingPotions();
    // Returns if the player used a potion on this frame. Used to activate the healing screen effect
    bool usedPotionInLastFrame();

    // Adds N coins to the player's money counter
    void pickCoin(int value);
    // Adds N kills to the player kill count
    void addKill(int amount);
    // Unlocks the specified weapon so you can use it. An unrecognized weapon string will do nothing
    void unlockWeapon(std::string weaponName);
    // Return true if the specified weapon is unlocked. An unrecognized weapon string will return false
    bool getIsWeaponUnlocked(std::string weaponName);

    // Virtual override: Function called when life points reach 0. In player's case this just means losing points, not death
    void die();
    // Virtual override: Function called when this collides with a Lethal Area (Hitbox type/Map area). In player's case this causes death, so the level will reset
    void trulyDie();

    // Sets the text for conversation that the nearest NPC has. This is stored in player to transfer this data to the TextState (Conversation state) later
    void setNear(std::vector<std::string> text);
    // Returns the text of the nearest NPC. This data is used to be showed on TextState (Conversation state)
    std::vector<std::string> getNear();

    // Sets the animation json file name we must read in the next animation (AnimationState)
    void setAnimationFilename(std::string filename);
    // Returns the animation json file name we must read in the next animation (AnimationState)
    std::string getAnimationFilename();

    // Sets the key index of the key we recovered in this level.
    void setRecoveredKey(int key);
    // Returns the key index of the key we recovered in this level. If we didn't get the key this returns -1
    int getRecoveredKey();

    // Virtual override: Sets the world's projectile array to its ranged weapon, if it has. The weapon will use this to add projectiles to the world
    void linkWorldProjectiles(std::vector<Projectile*>& proyectileArray);
    // Virtual override: Gets the hitbox type and returns if its a hitbox that we should collide with
    bool checkObstacle(Hitbox* hitbox);
    // Virtual override: Gets the hitbox type and returns if its a hitbox that should harm us
    bool checkInteraction(Hitbox* hitbox);
    // Virtual override: Resizes items to match the new size. Mainly used to adapt the base absolute entity to the tile size of the current map. Includes weapon size and reach, jump height, etc...
    void resizeItems(sf::Vector2f scaleRatio);

    // In game point modification, add specified points
    void addPoints(int add);
    // Substracts specified points, points cannot go below 0
    void substractPoints(int substract);
    // Returns the score (integer)
    int getPoints();

    // Save data getters and setters
    std::string getTotalCoins();
    std::string getMony();
    std::string getKills();
    std::string getDeaths();
    std::string getHealthUpg();
    std::string getLevel(); // Level name
    std::string getDoor();  // Level door
    std::string getHammrLvl();
    std::string getSwordLvl();
    std::string getShieldLvl();
    std::string getBowLvl();
    std::string getPotionsLeft();
    std::string getScore();
    std::string getKeyUnlocked(int index);
    void setTotalCoins(int i_money);
    void setMony(int i_money);
    void setKills(int i_kills);
    void setDeaths(int i_deaths);
    void setHealthUpg(int i_healthupg);
    void setLevel(std::string s_levelName);
    void setDoor(int i_door);
    void setHammrLvl(int i_lvl);
    void setSwordLvl(int i_lvl);
    void setShieldLvl(int i_lvl);
    void setBowLvl(int i_lvl);
    void setRemainingPotions(int i_scr);
    void setScore(int i_scr);
    void setKeyUnlocked(bool unlocked, int i_keyIndex);

    void updateMovement();
    void updateAnimation();

    void update();
    void render(float frameProgress);

};

#endif
