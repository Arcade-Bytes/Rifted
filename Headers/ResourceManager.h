#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Engine.h"
#include <unordered_map>
#include <queue>

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
    std::queue<sf::Sound*> soundstoplay;

    const std::string workspace = "./";

    //music and stuff
    sf::Music music_level;      //main level music: ambient or action.
    sf::Music music_transition; //transition music: build-ups or disengages to play while switching main level music.
    sf::Music music_level_alt;
    sf::Music music_menu;
    sf::Sound snd_critical_health;
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
     * Plays and stores the (buffer) sound given the filename of 
     * the file, no extension, just the name.
     **/
    void playSound(const std::string& filename);    //play the sound

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
     * set everything for the level 0 (tutorial and lobby) music
     * DO NOT CALL FROM OUTSIDE RESOURCEMANAGER
     **/
    void level0music();
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
     * Just plays the "recovered" key music
     * DO NOT CALL FROM OUTSIDE RESOURCEMANAGER
     **/
    void bossKeyAnimationMusic();
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
    /**
     * update the sound queue
     **/
    void soundUpdate();
    /**
     * And now the true test... hold fast, or expire?
     **/
    void PlayerCritical();
    /**
     * Many fall in the face of chaos, but not this one, not today
     **/
    void PlayerNotCritical();
};

#endif