#ifndef MAP_H
#define MAP_H

#include "Engine.h"
#include "Hitbox.h"
#include "../tinyxml2-master/tinyxml2.h"

using namespace tinyxml2;

struct MapObject{
    sf::Vector2f positon;
    sf::Vector2f size;
    int type;
    std::string name;
};

class Map {
private:
    // Resources
    sf::Texture* bgTexture;
    sf::Texture* tilesetTexture;

    // Tileset data
    sf::Vector2i v_tilesetGridSize;
    sf::Vector2i v_tilesetTileSize;
    int i_tilesetLinearSize;

    // Map Sizes
    unsigned int layers;
    sf::Vector2i v_gridSize;
    sf::Vector2i v_tileSize;
    sf::Vector2f scaleFactor;

    // Starting positions
    sf::Vector2f playerStartingPosition;
    std::vector<MapObject> enemyData;
    std::vector<MapObject> leverData;
    std::vector<MapObject> doorData;
    std::vector<MapObject> exitData;

<<<<<<< HEAD
    std::vector<std::vector<std::vector<sf::Sprite*> > > map;
=======
    std::vector<std::vector<std::vector<sf::Sprite*>>> map;
>>>>>>> d0f482223e1cd0fa998853caf699f2ef19fe23ae
    sf::Sprite* background;
    std::vector<Hitbox*> v_mapHitboxes;
public:
    Map(std::string filename, sf::Vector2i overrideTileSize, const int& entranceIndex);
    ~Map();

    // Starting position getters
    sf::Vector2f getPlayerPosition();
    std::vector<MapObject> getEnemyData();
    std::vector<MapObject> getLeverData();
    std::vector<MapObject> getDoorData();
    std::vector<MapObject> getExitData();

    void render();
};

#endif
