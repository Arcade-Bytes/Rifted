#include "ResourceManager.h"

ResourceManager* ResourceManager::resourceManager = NULL;

ResourceManager::ResourceManager()
{
    
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
