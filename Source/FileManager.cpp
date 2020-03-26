
#include "FileManager.h"

using namespace tinyxml2;   //fuck off long string >:((

namespace ftl{
    void ModifyByKey(std::string s_key, std::string s_value)
    {
        XMLDocument xml_save;
        OpenSaveFile(xml_save);
        xml_save.FirstChildElement(s_key.c_str())->SetText(s_value.c_str());
        CloseSaveFile(xml_save);
    }
    std::string LoadByKey(std::string s_key)
    {
        XMLDocument xml_load;
        OpenSaveFile(xml_load);
        std::string s_ReturnValue = xml_load.FirstChildElement(s_key.c_str())->GetText();
        CloseSaveFile(xml_load);
        return s_ReturnValue;
    }

    void SaveGame()
    {

    }

    void LoadGame()
    {
        
    }


    void OpenSaveFile(XMLDocument& xml_saveFile){
        if(xml_saveFile.LoadFile(SAVE_FILE) != 0)
        {
            std::cerr<< "error opening save file\n";
            exit(EXIT_FAILURE);
        }

    }
    void CloseSaveFile(XMLDocument& xml_saveFile){
        if(xml_saveFile.SaveFile(SAVE_FILE,true)!=0)
        {
            std::cerr<< "error saving save file\n";
            exit(EXIT_FAILURE);
        }

    }
    
    

};