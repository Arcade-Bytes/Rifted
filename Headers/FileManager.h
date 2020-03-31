#include <iostream>
#include <string.h>
#include "tinyxml2.h"
#include "Player.h"

#define SAVE_FILE   "../Rifted/ds000002.sl"
#define EXTRA_FILE  "ds000003.sl"

#define LEVEL1  ""

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
     * NOT IMPLEMENTED
     * 
     * 
     **/
    void PickedCoin(std::string s_level, int i_CoinId);
    /**
     * NOT IMPLEMENTED
     * 
     * 
     **/
    void ActivatedLever(std::string s_level, int i_LeverId);    
    /**
     * NOT IMPLEMENTED
     * 
     * 
     **/ 
    void SaveGame(Player &player);
    /**
     * NOT IMPLEMENTED
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

