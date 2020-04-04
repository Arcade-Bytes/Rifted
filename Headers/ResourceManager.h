#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Engine.h"
#include <unordered_map>

class ResourceManager
{
private:
    ResourceManager();
    ResourceManager(const ResourceManager& ResourceManager) = delete;
    ResourceManager& operator=(const ResourceManager& ResourceManager) = delete;
    ~ResourceManager();

    static ResourceManager* resourceManager;

    std::unordered_map<std::string, sf::Texture*> textures;
    std::unordered_map<std::string, sf::SoundBuffer*> sounds;
    std::unordered_map<std::string, sf::Font*> fonts;

    const std::string workspace = "./";
    
public:
    static ResourceManager* getInstance();

    // Cargar Texturas
    sf::Texture* loadTexture(const std::string& filepath);
    sf::Texture* getTexture(const std::string& filepath);
    void freeTexture(const std::string& filepath);

    // Cargar Audio
    sf::SoundBuffer* loadSound(const std::string& filepath);
    sf::SoundBuffer* getSound(const std::string& filepath);
    void freeSound(const std::string& filepath);

    // Cargar Texto
    sf::Font* loadFont(const std::string& filepath);
    sf::Font* getFont(const std::string& filepath);
    void freeFont(const std::string& filepath);
};

#endif