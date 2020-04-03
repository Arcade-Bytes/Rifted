#include <unistd.h>
#include <limits.h>
#include <iostream>
#include <string.h>
#include "../tinyxml2-master/tinyxml2.h"
#include "Player.h"

#define SFL_SAVE_FILE   "ds000002.sl"
#define SFL_EXTRA_FILE  "ds000003.sl"

#define SFL_SAVE "savefile"
#define SFL_PLAYER "player"
#define SFL_HEALTH "health"
#define SFL_MAXHEALTH "maxhealth"
#define SFL_COINS "coin"
#define SFL_DEATHS "deaths"
#define SFL_KILLS "kills"
#define SFL_UPG "upgrades"
#define SFL_LVL "level"
#define SFL_LVLNAME "name"
#define SFL_DOOR "door"
#define SFL_SHIELD "shield"
#define SFL_HMMR "hammr"
#define SFL_BOW "bow"
#define SFL_SWD "sword"
#define SFL_WPN "weapons"

#define SFL_ LEVEL1  ""

using namespace tinyxml2;   //fuck off long string >:((
    

/*
    A collection of functions to save and load data, that is reading
    and writting on the save files.Use generic access to read or 
    write on save files fornon-repetitive, non-specific tasks, 
    otherwise use the dedicated methods as they have been optimized
    for that specific purpose
    
*/
namespace ftl{

    /**
     * Modify a value given a key.
     * An early testing method and deprecated.
     * Does not see much use with the current save files...
     * */
    void ModifyByKey(std::string s_key, std::string s_value);
    /**
     * Modify a value given a hierarchical array of strings to the value
     * that must be modified, a general method that comes at more computational
     * cost, yet can access everything, that is, if the wizard knows the true path
     * 
     * (if you know where to look, this method will find it and modyfy it)
     * 
     * */
    void ModifyByKey(std::string s_key[], std::string s_value);
    /**
     * Returns the value AS A STRING located at key, given that s_key
     * has no descendants. An earlier version for testing purposes 
     * */
    std::string LoadByKey(std::string s_key);
    /**
     * Returns a string containing the value in s_key[size-1],
     * s_key must be an array in a hierarchial format being [0] the
     * first node and the next ones it's children 
     * 
     **/
    std::string LoadByKey(std::string s_key[]);
    /**
     * Sets to true (1) a picked up coin given the level and
     * the coin ID 
     * 
     **/
    void PickedCoin(std::string s_level, int i_CoinId);
    /**
     * Switches a lever, given the level and the lever ID 
     * 
     **/
    void LeverSwitch(std::string s_level, int i_LeverId);
    /**
     * Set the current picked state of a given coin
     **/ 
    void SetCoinState(std::string s_level, int i_CoinId, bool state);
    /**
     * Set the current picked state of a given lever
     **/ 
    void SetLeverState(std::string s_level, int i_LeverId, bool state);
    /**
     * Get the current picked state of a given coin
     **/ 
    bool GetCoinState(std::string s_level, int i_CoinId);
    /**
     * Get the current picked state of a given lever
     **/ 
    bool GetLeverState(std::string s_level, int i_LeverId);
    /**
     * ADD DESCRIPTION
     * 
     * 
     **/ 
    void SaveGame(Player &player);
    /**
     * ADD DESCRIPTION
     * 
     * 
     **/
    void LoadGame(Player &player);
    /**
     * Opens a XMLDocument 
     **/ 
    void OpenSaveFile(XMLDocument &xml_doc);
    /**
     * Closes/Saves a XMLDocument 
     **/ 
    void CloseSaveFile(XMLDocument &xml_doc);    

};

/*
    Programmer's Grimoire volume 1
    ******************************
        -Dark Magic 1:  With the reference to the array, we obtain the memory adress and then, we add 1. 
                        This gives us the next free memory adress, which happens to be an equal number of
                        positions away as the number of the elements in the array. And thus we subtract 
                        the first memory adress to obtain the differences, that is, the number of elements
                        in the array





*/
/*
    GUARDAR:
    pantalla (current level)
    mejoras equipadas
    herramientas con sus mejoras
    enemigos muertos
    puntos
    objetos debloqueados
    estado actual (salud, etc...).
         git config --global user.email "you@example.com"
         git config --global user.name "Your Name"

*/

