#ifndef PLAYER_H
#define PLAYER_H

#include "Engine.h"
#include "Entity.h"
#include "Weapon.h"
#include "Shield.h"

class Player : public Entity {
private:
    // Life regeneration properties
    float f_regenerationDelta;      // Time since last damage/healing action
    float f_regenerationCooldown;   // Necessary time to start regenerating
    float f_regenerationSpeed;      // Time between regen and regen once you're already regenerating
    float f_regenerationAmount;     // Amount of life regenerated every update
    int i_nchunks;                  // Number of chunks the lifebar is divided in
    int i_coins;                    // Amount of coins
    int i_deaths;                   // Amount of player deaths... does it even die?
    int i_kills;                    // Amount of kills
    int i_door;                     // Door we used to enter the level
    int i_healthUpg;                // Current health level, multiplier?
    std::string s_levelName;        // Current level name
    

    Weapon* sword;
    Weapon* hammer;
    Weapon* bow;
    Shield* shield;
public:
    Player();
    ~Player();

    void getHurt(float& damage);
    void getHealed(float& healing);
    void regenerate();

    /**
     * Returns how much money the player has
     **/
    std::string getMony();
    /**
     * Returns how many souls the player has reaped
     **/
    std::string getKills();
    /**
     * Returns how many times the player has DIED
     **/
    std::string getDeaths();
    /**
     * Returns how much the health has been upgraded
     **/
    std::string getHealthUpg();
    /**
     * Returns the current level the player is in
     **/
    std::string getLevel();
    /**
     * Returns the door the player entered the level from
     **/
    std::string getDoor();
    /**
     * Returns the upgrade level of the HÄMMRR
     **/
    std::string getHammrLvl();
    /**
     * Returns the upgrade level of the sword
     **/
    std::string getSwordLvl();
    /**
     * Returns the upgrade level of the shield
     **/
    std::string getShieldLvl();
    /**
     * Returns the upgrade level of the bow
     **/
    std::string getBowLvl();
    /**
     * Sets how much money the player has
     **/
    void setMony(int i_money);
    /**
     * Sets how many souls the player has reaped
     **/
    void setKills(int i_kills);
    /**
     * Sets how many times the player has DIED
     **/
    void setDeaths(int i_deaths);
    /**
     * Sets how much the health has been upgraded
     **/
    void setHealthUpg(int i_healthupg);
    /**
     * Sets the current level the player is in
     **/
    void setLevel(std::string s_levelName);
    /**
     * Sets the door the player entered the level from
     **/
    void setDoor(int i_door);
    /**
     * Sets the upgrade level of the HÄMMRR
     **/
    void setHammrLvl(int i_lvl);
    /**
     * Sets the upgrade level of the sword
     **/
    void setSwordLvl(int i_lvl);
    /**
     * Sets the upgrade level of the shield
     **/
    void setShieldLvl(int i_lvl);
    /**
     * Sets the upgrade level of the bow
     **/
    void setBowLvl(int i_lvl);

    void update();
    void render();
};

#endif
