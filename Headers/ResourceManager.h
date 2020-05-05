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

    //music and stuff
    sf::Music music_level;      //main level music: ambient or action.
    sf::Music music_transition; //transition music: build-ups or disengages to play while switching main level music.
    sf::Music music_level_alt;
    sf::Music music_menu;
    int i_level;                //tells the level so we can manage music, set on playLevelMusic()
    enum MusicStatus{act,amb};
    MusicStatus mstatus;
    
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
    /**
     * Theorically speaking the sound will play before we destroy the
     * variable sf::Sound, the buffer is never destroyed so it should play
     * just fine.
     **/
    void playSound(const std::string& filepath);    //play the sound

    // Cargar Texto
    sf::Font* loadFont(const std::string& filepath);
    sf::Font* getFont(const std::string& filepath);
    void freeFont(const std::string& filepath);

    //fun and music
    void playLevelMusic(std::string levelName);
    /**
     * start playing the main menu music
     **/
    void playMainMenu();
    /**
     * stop playing the main menu music,
     * it is set to STOPPED so it can start
     * from the begining again
     **/
    void stopMainMenu();

    //music and stuff functions

    /**
     * set everything for the level 1 (mine) music
     * DO NOT CALL FROM OUTSIDE RESOURCEMANAGER
     **/
    void level1music();
    /**
     * set everything for the level 2 (graveyard) music
     * DO NOT CALL FROM OUTSIDE RESOURCEMANAGER
     **/
    void level2music();
    /**
     * set everything for the level 3 (tower) music
     * DO NOT CALL FROM OUTSIDE RESOURCEMANAGER
     **/
    void level3music();
    /**
     * manages level music
     **/
    void musicUpdate();
    /**
     * manages level 1 music
     * DO NOT CALL FROM OUTSIDE RESOURCEMANAGER
     **/
    void level1Update();
    /**
     * slow down the music
     **/
    void MusicToMellow();
    /**
     * kick the music in
     **/
    void MusicToAction();
};

#endif