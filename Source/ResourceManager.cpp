#include "ResourceManager.h"

ResourceManager* ResourceManager::resourceManager = NULL;

ResourceManager::ResourceManager()
{
    this->music_menu.openFromFile("resources/still_alive.wav");
    this->music_menu.setVolume(50.f);
    this->music_menu.setLoop(true);
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

void ResourceManager::playSound(const std::string& filepath)
{
    sf::SoundBuffer* sndb_toPlay = this->getSound(filepath);
    if(sndb_toPlay != NULL)
    {
        sf::Sound snd_play;
        snd_play.setBuffer(*sndb_toPlay);
        snd_play.play();
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
    if(levelName.compare("Prueba_Beta")==0)
        this->level1music();    //also set i_level
}

void ResourceManager::level1music()
{
    this->i_level = 0;
    this->music_level.openFromFile("resources/Burn_in_Hell.flac");
    this->music_level.play();
}

void ResourceManager::musicUpdate()
{
    switch (this->i_level)
    {
    case 0: //level 1 (mine)
        this->level1Update();
        break;
    
    default:
        break;
    }

}
/* Hard Code Café */
void ResourceManager::level1Update()
{
    if(this->music_level.getPlayingOffset().asMicroseconds()-215290000.f>=0)
    {
        //this->music_level.setPlayingOffset(sf::Time(sf::seconds(0.f)));
        this->music_level.setPlayingOffset(sf::Time(sf::seconds(21.09f)));
        //this->music_level.play();
    }
}

