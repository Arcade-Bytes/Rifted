#include "ResourceManager.h"

ResourceManager* ResourceManager::resourceManager = NULL;

ResourceManager::ResourceManager()
{
    this->music_menu.openFromFile("resources/still_alive.wav");
    this->music_menu.setVolume(30.f);
    this->music_level.setVolume(40.f);
    this->music_level_alt.setVolume(40.f);
    this->music_transition.setVolume(40.f);
    this->music_menu.setLoop(true);
    this->music_level_alt.setLoop(true);
    this->music_level.setLoop(true);
    this->mstatus = ResourceManager::MusicStatus::amb;
    this->snd_critical_health.setBuffer(*this->loadSound("resources/sounds/heartbeat_long_eco.wav"));
    this->snd_critical_health.setLoop(true);
}

// Singleton
ResourceManager* ResourceManager::getInstance()
{
    if(!ResourceManager::resourceManager)
        ResourceManager::resourceManager = new ResourceManager();

    return ResourceManager::resourceManager;
}

// Borra las texturas, sonidos y textos
ResourceManager::~ResourceManager()
{
    for (auto iter = textures.begin(); iter != textures.end(); iter++) delete iter->second;
    for (auto iter = sounds.begin(); iter != sounds.end(); iter++) delete iter->second;
    for (auto iter = fonts.begin(); iter != fonts.end(); iter++) delete iter->second;

    while(!soundstoplay.empty())
    {
        delete soundstoplay.front();
        soundstoplay.pop();
    }
}

// Carga de texturas
sf::Texture* ResourceManager::loadTexture(const std::string& filepath)
{
    // Si ya existía se devuelve del unordered_map
    if(this->textures.find(filepath) != this->textures.end())
        return textures[filepath];
    
    // Si no se trata de cargar y guardar en el unordered_map
    sf::Texture* texture = new sf::Texture();

    if(texture->loadFromFile(this->workspace + filepath))
        textures[filepath] = texture;

    return texture;
}

sf::Texture* ResourceManager::getTexture(const std::string& filepath)
{
    // Si ya existía se devuelve del unordered_map
    if (textures.find(filepath) != textures.end())
        return textures[filepath];

    // Si no, null pointer
    return NULL;
}

void ResourceManager::freeTexture(const std::string& filepath)
{
    auto iter = textures.find(filepath);

    if (iter != textures.end()) {
        delete iter->second;
        textures.erase(iter);
    }
}

// Cargar Audio
sf::SoundBuffer* ResourceManager::loadSound(const std::string& filepath)
{
    if(this->sounds.find(filepath) != this->sounds.end())
        return sounds[filepath];
    
    sf::SoundBuffer* sound = new sf::SoundBuffer();

    if(sound->loadFromFile(this->workspace + filepath))
        sounds[filepath] = sound;
    
    return sound;
}

sf::SoundBuffer* ResourceManager::getSound(const std::string& filepath)
{
    if (sounds.find(filepath) != sounds.end())
        return sounds[filepath];

    return NULL;
}

void ResourceManager::freeSound(const std::string& filepath)
{
    auto iter = sounds.find(filepath);

    if (iter != sounds.end()) {
        delete iter->second;
        sounds.erase(iter);
    }
}

// Cargar Texto
sf::Font* ResourceManager::loadFont(const std::string& filepath)
{
    if(this->fonts.find(filepath) != this->fonts.end())
        return fonts[filepath];
    
    sf::Font* font = new sf::Font();

    if(font->loadFromFile(this->workspace + filepath))
        fonts[filepath] = font;
    
    return font;
}

sf::Font* ResourceManager::getFont(const std::string& filepath)
{
    if (fonts.find(filepath) != fonts.end())
        return fonts[filepath];

    return NULL;
}

void ResourceManager::freeFont(const std::string& filepath)
{
    auto iter = fonts.find(filepath);

    if (iter != fonts.end()) {
        delete iter->second;
        fonts.erase(iter);
    }
}
//play sound and music

void ResourceManager::playSound(const std::string& filename)
{
    sf::SoundBuffer* sndb_toPlay = this->loadSound("resources/sounds/"+filename+".wav");
    if(sndb_toPlay != NULL)
    {        
        sf::Sound* snd_play= new sf::Sound();
        snd_play->setBuffer(*sndb_toPlay);
        
        this->soundstoplay.push(snd_play);  
        this->soundstoplay.back()->play();     
    }

}

void ResourceManager::playMainMenu()
{
    //stop everything playing if we're coming from a level
    if(this->music_level_alt.getStatus() == sf::Music::Playing)
        this->music_level_alt.stop();
    if(this->music_transition.getStatus() == sf::Music::Playing)
        this->music_transition.stop();
    if(this->music_level.getStatus() == sf::Music::Playing)
        this->music_level.stop();
    //now we choose to play the main menu music
    if(this->music_menu.getStatus() != sf::Music::Playing)
    {
        this->music_menu.play();
    }
}
void ResourceManager::stopMainMenu()
{
    if(this->music_menu.getStatus() == sf::Music::Playing)
    {
        this->music_menu.stop();
    }

}

void ResourceManager::playLevelMusic(std::string levelName)
{
    if(levelName.compare("mina")==0)
        this->level1music();    //also set i_level
    else if(levelName.compare("cementerio")==0)
        this->level2music();
    else if(levelName.compare("torre")==0)
        this->level3music();
    else if(levelName.compare("tutorial")==0)
        this->level0music();
}
/* START OF LEVEL MUSIC SET UPS*/
void ResourceManager::level0music()
{
    this->i_level = -1;
    this->music_level.openFromFile("resources/dust_to_dust.wav");
    this->music_level_alt.openFromFile("resources/dust_to_dust.wav");
    this->music_transition.openFromFile("resources/dust_to_dust.wav");
    this->music_level.play();
    this->music_transition.play();
    this->music_transition.pause();
}
void ResourceManager::level1music()
{
    this->i_level = 0;
    this->music_level.openFromFile("resources/burn_in_hell_calm.wav");
    this->music_level_alt.openFromFile("resources/burn_in_hell_act_loop.wav");
    this->music_transition.openFromFile("resources/silence.wav");
    this->music_level.play();
    this->music_transition.play();
    this->music_transition.pause();
}
void ResourceManager::level2music()
{
    this->i_level = 1;
    this->music_level.openFromFile("resources/graveyard_amb_tension_hideous.wav");                //calm track
    this->music_transition.openFromFile("resources/graveyard_act_loop_intro.wav");      //transition to action
    this->music_level_alt.openFromFile("resources/graveyard_act_loop.wav");
    this->music_level.play();
    this->music_transition.play();
    this->music_transition.pause();
}
void ResourceManager::level3music()
{
    this->i_level = 2;
    this->music_level.openFromFile("resources/6_Mellow_loop.wav");                //calm track
    this->music_transition.openFromFile("resources/3_Pre_action_track.wav");      //transition to action
    this->music_level_alt.openFromFile("resources/4_action_loop_1.wav");
    this->music_level.play();
    this->music_transition.play();
    this->music_transition.pause();
}
/*END OF LEVEL MUSIC SET UPS*/

/* SOUND UPDATE */
void ResourceManager::soundUpdate()
{
    //std::cout << this->soundstoplay.size() << "\n";
    if(!this->soundstoplay.empty())
    {
        if(this->soundstoplay.front()->getStatus() != sf::Music::Playing)
        {
            this->soundstoplay.pop();
        }
    }
}
/* END OF SOUND UPDATE */

/*START OF MUSIC UPDATE*/

void ResourceManager::musicUpdate()
{
    switch (this->i_level)
    {
    case 0: //level 1 (mine)
        this->level1Update();
        break;
    case 1:
        switch (this->mstatus)
        {
            case ResourceManager::MusicStatus::amb:
                if(this->music_level.getStatus()==sf::Music::Stopped)
                {
                    this->music_level.setVolume(0.f);
                    this->music_level.play();
                    
                }
                if(this->music_transition.getStatus() == sf::Music::Playing)
                {
                    this->music_level.setVolume(this->music_transition.getPlayingOffset().asSeconds()*100/23.f);
                }
                else
                {
                    this->music_transition.openFromFile("resources/graveyard_act_loop_intro.wav");
                }
                
                break;
            case ResourceManager::MusicStatus::act:
                if(this->music_transition.getStatus()==sf::Music::Stopped && this->music_level_alt.getStatus()==sf::Music::Stopped)
                {
                    this->music_level.stop();
                    this->music_level_alt.play();
                    this->music_transition.openFromFile("resources/graveyard_act_loop ending.wav");
                }
                break;
        }
        break;
    case 2:
        switch (this->mstatus)
        {
            case ResourceManager::MusicStatus::amb:
                if(this->music_transition.getStatus()==sf::Music::Stopped && this->music_level.getStatus()==sf::Music::Stopped)
                {
                    this->music_level.play();
                    this->music_transition.openFromFile("resources/3_Pre_action_track.wav");
                }
                break;
            case ResourceManager::MusicStatus::act:
                if(this->music_transition.getStatus()==sf::Music::Stopped && this->music_level_alt.getStatus()==sf::Music::Stopped)
                {
                    this->music_level.stop();
                    this->music_level_alt.play();
                    this->music_transition.openFromFile("resources/5_action_to_mellow.wav");
                }
                break;
        }
        break;
    
    default:
        break;
    }

}
void ResourceManager::MusicToAction()
{
    this->mstatus = ResourceManager::MusicStatus::act;
    if(this->music_transition.getStatus() != sf::Music::Playing && this->music_level_alt.getStatus() != sf::Music::Playing && this->i_level != 0)
        this->music_transition.play();

}
void ResourceManager::MusicToMellow()
{
    this->mstatus = ResourceManager::MusicStatus::amb;
    if(this->music_transition.getStatus() != sf::Music::Playing && this->music_level.getStatus() != sf::Music::Playing && this->i_level != 0)
    {
        this->music_transition.play();
        this->music_level_alt.stop();
    }
    
}
/*END OF MUSIC UPDATE*/

/*PLAYER CRIT*/
void ResourceManager::PlayerCritical()
{
    if(this->snd_critical_health.getStatus() != sf::Sound::Playing)
    {
    this->playSound("gen_combat_deathsdoor");
    this->snd_critical_health.play();
    
    }
}

void ResourceManager::PlayerNotCritical()
{
    if(this->snd_critical_health.getStatus() == sf::Sound::Playing)
        this->snd_critical_health.stop();
}
/*END OF PLAYER CRIT*/
/* Hard Code Café */
void ResourceManager::level1Update()
{
    if(this->mstatus == ResourceManager::MusicStatus::act)
    {
        this->music_level.setLoop(false);
    }
    if(this->music_level.getStatus() == sf::Music::Stopped && this->music_level_alt.getStatus() == !sf::Music::Playing)
    {
        this->music_level_alt.play();
    }
    
}

