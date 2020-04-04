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
    int i_score;                    // Score
    std::string s_levelName;        // Current level name
    

    Weapon* sword;
    Weapon* hammer;
    Weapon* bow;
    Shield* shield;
public:
    Player(const float& maxHealth);
    ~Player();

    void getHurt(float& damage);
    void getHealed(float& healing);
    void regenerate();

    // Virtual override
    bool checkObstacle(Hitbox* hitbox);
    bool checkInteraction(Hitbox* hitbox);
    void resizeItems(sf::Vector2f scaleRatio);

    // Save data getters and setters
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
    std::string getScore();
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

    void update();
    void render();
};

#endif
