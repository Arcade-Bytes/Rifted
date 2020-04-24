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
        std::string s_coinId = "c"+std::to_string(i_CoinId);
        XMLDocument xml_doc;
        OpenSaveFile(xml_doc);       
        xml_doc.FirstChildElement(s_level.c_str())->FirstChildElement(s_coinId.c_str())->SetText("1");
        CloseSaveFile(xml_doc);
    }

    void LeverSwitch(std::string s_level, int i_LeverId)
    {
        std::string s_LeverId = "l"+std::to_string(i_LeverId);
        XMLDocument xml_doc;
        OpenSaveFile(xml_doc);       
        std::string s_prev = xml_doc.FirstChildElement(s_level.c_str())->FirstChildElement( s_LeverId.c_str())->GetText();
        if(std::atoi(s_prev.c_str()))
            xml_doc.FirstChildElement(s_level.c_str())->FirstChildElement( s_LeverId.c_str())->SetText("1");
        else
            xml_doc.FirstChildElement(s_level.c_str())->FirstChildElement( s_LeverId.c_str())->SetText("0");
        CloseSaveFile(xml_doc);  
    }

    void SetCoinState(std::string s_level, int i_CoinId, bool state)
    {
        std::string s_coinId = "c"+std::to_string(i_CoinId);
        XMLDocument xml_doc;
        OpenSaveFile(xml_doc); 

        XMLElement* level = xml_doc.FirstChildElement(s_level.c_str());
        if(!level)
        {
            xml_doc.InsertEndChild(xml_doc.NewElement(s_level.c_str()));
            level = xml_doc.FirstChildElement(s_level.c_str());
        }
        XMLElement* coinData = level->FirstChildElement(s_coinId.c_str());
        if(!coinData)
        {
            level->InsertEndChild(xml_doc.NewElement(s_coinId.c_str()));
            coinData = level->FirstChildElement(s_coinId.c_str());
        }
        coinData->SetText(state ? "1" : "0");

        CloseSaveFile(xml_doc); 
    }

    void SetLeverState(std::string s_level, int i_LeverId, bool state)
    {
        std::string s_LeverId = "l"+std::to_string(i_LeverId);
        XMLDocument xml_doc;
        OpenSaveFile(xml_doc);

        XMLElement* level = xml_doc.FirstChildElement(s_level.c_str());
        if(!level)
        {
            xml_doc.InsertEndChild(xml_doc.NewElement(s_level.c_str()));
            level = xml_doc.FirstChildElement(s_level.c_str());
        }
        XMLElement* leverData = level->FirstChildElement(s_LeverId.c_str());
        if(!leverData)
        {
            level->InsertEndChild(xml_doc.NewElement(s_LeverId.c_str()));
            leverData = level->FirstChildElement(s_LeverId.c_str());
        }
        leverData->SetText(state ? "1" : "0");      
        
        CloseSaveFile(xml_doc); 
    }

    bool GetCoinState(std::string s_level, int i_CoinId)
    {
        std::string s_coinId = "c"+std::to_string(i_CoinId);
        int state = 0;
        XMLDocument xml_doc;
        OpenSaveFile(xml_doc);

        XMLElement* level = xml_doc.FirstChildElement(s_level.c_str());
        if(level)
        {
            XMLElement* coinData = level->FirstChildElement( s_coinId.c_str());
            if(coinData)
                state = std::atoi(coinData->GetText());
        } 

        CloseSaveFile(xml_doc); 

        return state == 0 ? false : true;
    }

    bool GetLeverState(std::string s_level, int i_LeverId)
    {
        std::string s_LeverId = "l"+std::to_string(i_LeverId);
        int state = 0;

        XMLDocument xml_doc;
        OpenSaveFile(xml_doc); 

        XMLElement* level = xml_doc.FirstChildElement(s_level.c_str());
        if(level)
        {
            XMLElement* leverData = level->FirstChildElement( s_LeverId.c_str());
            if(leverData)
                state = std::atoi(leverData->GetText());
        }   
        
        CloseSaveFile(xml_doc); 

        return state == 0 ? false : true;
    }

    void SaveGame(Player &player)
    {
        //coins
        XMLDocument xml_doc;
        OpenSaveFile(xml_doc);
        //coins
        xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_COINS)->SetText(player.getMony().c_str());
        //health
        xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_HEALTH)->SetText(player.getHealth().c_str());
        //maxhealth
        xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_MAXHEALTH)->SetText(player.getMaxHealth().c_str());
        //remaining potions
        xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_POTIONS)->SetText(player.getPotionsLeft().c_str());
        //kills
        xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_KILLS)->SetText(player.getKills().c_str());
        //deaths
        xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_DEATHS)->SetText(player.getDeaths().c_str());
        //score
        xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_SCORE)->SetText(player.getScore().c_str());
        //upgrades
        xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_UPG)->FirstChildElement(SFL_HEALTH)->SetText(player.getHealthUpg().c_str());
        //level
        xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_LVL)->FirstChildElement(SFL_LVLNAME)->SetText(player.getLevel().c_str());
        //door
        xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_LVL)->FirstChildElement(SFL_DOOR)->SetText(player.getDoor().c_str());
        //weapons
        //shield
        xml_doc.FirstChildElement(SFL_WPN)->FirstChildElement(SFL_SHIELD)->FirstChildElement(SFL_UPG)->SetText(player.getShieldLvl().c_str());
        //hammr
        xml_doc.FirstChildElement(SFL_WPN)->FirstChildElement(SFL_HMMR)->FirstChildElement(SFL_UPG)->SetText(player.getHammrLvl().c_str());
        //bow
        xml_doc.FirstChildElement(SFL_WPN)->FirstChildElement(SFL_BOW)->FirstChildElement(SFL_UPG)->SetText(player.getBowLvl().c_str());
        //sword?
        xml_doc.FirstChildElement(SFL_WPN)->FirstChildElement(SFL_SWD)->FirstChildElement(SFL_UPG)->SetText(player.getSwordLvl().c_str());

        CloseSaveFile(xml_doc);


    }

    void LoadGame(Player &player)
    {
        XMLDocument xml_doc;
        OpenSaveFile(xml_doc);
        //coins
        player.setMony(std::stoi(xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_COINS)->GetText()));
        //health
        player.setHealth(std::stof(xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_HEALTH)->GetText()));
        //maxhealth
        player.setMaxHealth(std::stof(xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_MAXHEALTH)->GetText())); 
        //remaining potions
        player.setRemainingPotions(std::stof(xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_POTIONS)->GetText()));       
        //kills
        player.setKills(std::stoi(xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_KILLS)->GetText()));        
        //deaths?
        player.setDeaths(std::stoi(xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_DEATHS)->GetText()));  
        //score
        player.setScore(std::stof(xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_SCORE)->GetText()));         
        //upgrades
        player.setHealthUpg(std::stoi(xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_UPG)->FirstChildElement(SFL_HEALTH)->GetText()));        
        //level
        player.setLevel(xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_LVL)->FirstChildElement(SFL_LVLNAME)->GetText());        
        //door
        player.setDoor(std::stoi(xml_doc.FirstChildElement(SFL_PLAYER)->FirstChildElement(SFL_LVL)->FirstChildElement(SFL_DOOR)->GetText()));        
        //weapons
        //shield
        player.setShieldLvl(std::stoi(xml_doc.FirstChildElement(SFL_WPN)->FirstChildElement(SFL_SHIELD)->FirstChildElement(SFL_UPG)->GetText()));
        //hammr
        player.setHammrLvl(std::stoi(xml_doc.FirstChildElement(SFL_WPN)->FirstChildElement(SFL_HMMR)->FirstChildElement(SFL_UPG)->GetText()));
        //bow
        player.setBowLvl(std::stoi(xml_doc.FirstChildElement(SFL_WPN)->FirstChildElement(SFL_BOW)->FirstChildElement(SFL_UPG)->GetText()));
        //sword?
        player.setSwordLvl(std::stoi(xml_doc.FirstChildElement(SFL_WPN)->FirstChildElement(SFL_SWD)->FirstChildElement(SFL_UPG)->GetText()));
        CloseSaveFile(xml_doc);
        
    }


    void OpenSaveFile(XMLDocument& xml_saveFile){
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
        //printf("Current working dir: %s\n", cwd);
        }
        int i_error = xml_saveFile.LoadFile(SFL_SAVE_FILE);
        
        if( i_error != 0)
        {
            std::cerr<< "error opening save file\n";
            xml_saveFile.PrintError();
            exit(EXIT_FAILURE);
        }

    }
    
    void CloseSaveFile(XMLDocument& xml_saveFile){
        if(xml_saveFile.SaveFile(SFL_SAVE_FILE,true)!=0)
        {
            std::cerr<< "error saving save file\n";
            exit(EXIT_FAILURE);
        }
    }

    void ResetSaveFile()
    {
        std::ifstream streamI;
        std::ofstream streamO;
        char ch;
        streamI.open(SFL_TEMPLATE_FILE);
        if(!streamI)
        {
            std::cout<<"Error in opening template file..!!\n";
            return;
        }
        streamO.open(SFL_SAVE_FILE, std::ofstream::out | std::ofstream::trunc);
        if(!streamO)
        {
            std::cout<<"Error in opening save file..!!\n";
            streamO.close();
            return;
        }
        streamI>>ch;
        while(streamI.eof()==0)
        {
            streamO<<ch;
            streamI>>ch;
        }
        streamI.close();
        streamO.close();
    }
};