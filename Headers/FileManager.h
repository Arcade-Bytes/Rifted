#include <iostream>
#include "tinyxml2.h"

#define SAVE_FILE   "../Rifted/ds000002.sl"
#define EXTRA_FILE  "ds000003.sl"

using namespace tinyxml2;   //fuck off long string >:((
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
namespace ftl{
    void ModifyByKey(std::string , std::string);
    std::string LoadByKey(std::string);
    void SaveGame();
    void LoadGame();
    void OpenSaveFile(XMLDocument&);
    void CloseSaveFile(XMLDocument&);    

};

