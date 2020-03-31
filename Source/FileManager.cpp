
#include <array>
#include "FileManager.h"

using namespace tinyxml2;   

namespace ftl{
    
    void ModifyByKey(std::string s_key, std::string s_value)
    {
        XMLDocument xml_save;
        OpenSaveFile(xml_save);
        xml_save.FirstChildElement(s_key.c_str())->SetText(s_value.c_str());
        CloseSaveFile(xml_save);
    }
    
    void ModifyByKey(std::string s_key[], std::string s_value)
    {
        XMLDocument xml_save;
        OpenSaveFile(xml_save);
        XMLNode *tag = xml_save.FirstChildElement(s_key[0].c_str());
        int i_size = *(&s_key + 1)-s_key;   //dark magic 1: see the header for more information
        i_size--;  
        if(i_size>1)
        {
            for(int i = 1;i<i_size;++i)
            {                
                tag = tag->ToElement()->FirstChildElement(s_key[i].c_str());
            }
        }
        tag->ToElement()->SetText(s_value.c_str());
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
 
    std::string LoadByKey(std::string s_key[])
    {
        XMLDocument xml_load;
        OpenSaveFile(xml_load); 
        XMLNode *tag = xml_load.FirstChildElement(s_key[0].c_str());

        int i_size = *(&s_key + 1)-s_key;   //dark magic 1: see the header for more information   
        i_size--;                                 
                                            
        if(i_size>1)
        {
            for(int i = 1;i<i_size;++i)
            {                
                tag = tag->ToElement()->FirstChildElement(s_key[i].c_str());
            }
        }
        return tag->ToElement()->GetText();
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