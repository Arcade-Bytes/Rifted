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

    void PickedCoin(std::string s_level, int i_CoinId)
    {
        std::string s_coinId = "a"+std::to_string(i_CoinId);
        XMLDocument xml_doc;
        OpenSaveFile(xml_doc);       
        xml_doc.FirstChildElement(s_level.c_str())->FirstChildElement(s_coinId.c_str())->SetText("1");
        CloseSaveFile(xml_doc);
    }

    void LeverSwitch(std::string s_level, int i_LeverId)
    {
        std::string s_LeverId = "a"+std::to_string(i_LeverId);
        XMLDocument xml_doc;
        OpenSaveFile(xml_doc);       
        std::string s_prev = xml_doc.FirstChildElement(s_level.c_str())->FirstChildElement( s_LeverId.c_str())->GetText();
        if(std::atoi(s_prev.c_str()))
            xml_doc.FirstChildElement(s_level.c_str())->FirstChildElement( s_LeverId.c_str())->SetText("1");
        else
            xml_doc.FirstChildElement(s_level.c_str())->FirstChildElement( s_LeverId.c_str())->SetText("0");
        CloseSaveFile(xml_doc);  
    }

    bool GetCoinState(std::string s_level, int i_CoinId)
    {
        std::string s_coinId = "a"+std::to_string(i_CoinId);
        XMLDocument xml_doc;
        OpenSaveFile(xml_doc);       
        std::string s_prev = xml_doc.FirstChildElement(s_level.c_str())->FirstChildElement( s_coinId.c_str())->GetText();
        int state = std::atoi(s_prev.c_str());
        CloseSaveFile(xml_doc); 

        return state == 0 ? false : true;
    }

    bool GetLeverState(std::string s_level, int i_LeverId)
    {
        std::string s_LeverId = "a"+std::to_string(i_LeverId);
        XMLDocument xml_doc;
        OpenSaveFile(xml_doc);       
        std::string s_prev = xml_doc.FirstChildElement(s_level.c_str())->FirstChildElement( s_LeverId.c_str())->GetText();
        int state = std::atoi(s_prev.c_str());
        CloseSaveFile(xml_doc); 

        return state == 0 ? false : true;
    }

    void SaveGame(Player &player)
    {
        //coins
        XMLDocument xml_doc;
        OpenSaveFile(xml_doc);
        //coins
        xml_doc.FirstChildElement(PLAYER)->FirstChildElement(COINS)->SetText(player.getMony().c_str());
        //health
        xml_doc.FirstChildElement(PLAYER)->FirstChildElement(HEALTH)->SetText(player.getHealth().c_str());
        //maxhealth
        xml_doc.FirstChildElement(PLAYER)->FirstChildElement(MAXHEALTH)->SetText(player.getMaxHealth().c_str());
        //kills
        xml_doc.FirstChildElement(PLAYER)->FirstChildElement(KILLS)->SetText(player.getKills().c_str());
        //deaths?
        xml_doc.FirstChildElement(PLAYER)->FirstChildElement(DEATHS)->SetText(player.getDeaths().c_str());
        //score?
        //  xml_doc.FirstChildElement(PLAYER)->FirstChildElement()->SetText();
        //upgrades
        xml_doc.FirstChildElement(PLAYER)->FirstChildElement(UPG)->FirstChildElement(HEALTH)->SetText(player.getHealthUpg().c_str());
        //level
        xml_doc.FirstChildElement(PLAYER)->FirstChildElement(LVL)->FirstChildElement(LVLNAME)->SetText(player.getLevel().c_str());
        //door
        xml_doc.FirstChildElement(PLAYER)->FirstChildElement(LVL)->FirstChildElement(DOOR)->SetText(player.getDoor().c_str());
        //weapons
        //shield
        xml_doc.FirstChildElement(WPN)->FirstChildElement(SHIELD)->FirstChildElement(UPG)->SetText(player.getShieldLvl().c_str());
        //hammr
        xml_doc.FirstChildElement(WPN)->FirstChildElement(HMMR)->FirstChildElement(UPG)->SetText(player.getHammrLvl().c_str());
        //bow
        xml_doc.FirstChildElement(WPN)->FirstChildElement(BOW)->FirstChildElement(UPG)->SetText(player.getBowLvl().c_str());
        //sword?
        xml_doc.FirstChildElement(WPN)->FirstChildElement(SWD)->FirstChildElement(UPG)->SetText(player.getSwordLvl().c_str());

        CloseSaveFile(xml_doc);


    }

    void LoadGame(Player &player)
    {
        XMLDocument xml_doc;
        OpenSaveFile(xml_doc);
        //coins
        player.setMony(std::stoi(xml_doc.FirstChildElement(PLAYER)->FirstChildElement(COINS)->GetText()));
        //health
        player.setHealth(std::stof(xml_doc.FirstChildElement(PLAYER)->FirstChildElement(HEALTH)->GetText()));
        //maxhealth
        player.setMaxHealth(std::stof(xml_doc.FirstChildElement(PLAYER)->FirstChildElement(MAXHEALTH)->GetText()));        
        //kills
        player.setKills(std::stoi(xml_doc.FirstChildElement(PLAYER)->FirstChildElement(KILLS)->GetText()));        
        //deaths?
        player.setDeaths(std::stoi(xml_doc.FirstChildElement(PLAYER)->FirstChildElement(DEATHS)->GetText()));        
        //upgrades
        player.setHealthUpg(std::stoi(xml_doc.FirstChildElement(PLAYER)->FirstChildElement(UPG)->FirstChildElement(HEALTH)->GetText()));        
        //level
        player.setLevel(xml_doc.FirstChildElement(PLAYER)->FirstChildElement(LVL)->FirstChildElement(LVLNAME)->GetText());        
        //door
        player.setDoor(std::stoi(xml_doc.FirstChildElement(PLAYER)->FirstChildElement(LVL)->FirstChildElement(DOOR)->GetText()));        
        //weapons
        //shield
        player.setShieldLvl(std::stoi(xml_doc.FirstChildElement(WPN)->FirstChildElement(SHIELD)->FirstChildElement(UPG)->GetText()));
        //hammr
        player.setHammrLvl(std::stoi(xml_doc.FirstChildElement(WPN)->FirstChildElement(HMMR)->FirstChildElement(UPG)->GetText()));
        //bow
        player.setBowLvl(std::stoi(xml_doc.FirstChildElement(WPN)->FirstChildElement(BOW)->FirstChildElement(UPG)->GetText()));
        //sword?
        player.setSwordLvl(std::stoi(xml_doc.FirstChildElement(WPN)->FirstChildElement(SWD)->FirstChildElement(UPG)->GetText()));
        CloseSaveFile(xml_doc);
        
    }


    void OpenSaveFile(XMLDocument& xml_saveFile){
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
        //printf("Current working dir: %s\n", cwd);
        }
        int i_error = xml_saveFile.LoadFile("ds000002.sl");
        
        if( i_error != 0)
        {
            std::cerr<< "error opening save file\n";
            xml_saveFile.PrintError();
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