#ifndef MAP_H
#define MAP_H

#include "Engine.h"
#include "Hitbox.h"
#include "ResourceManager.h"
#include <tinyxml2.h>

using namespace tinyxml2;

struct MapObject{
    sf::Vector2f positon;
    sf::Vector2f size;
    int type;
    std::string name;
};

class Map {
private:
    // Tileset data
    sf::Vector2i v_tilesetGridSize;
    sf::Vector2i v_tilesetTileSize;
    sf::Vector2f v_totalPixelSize;
    int i_tilesetLinearSize;

    // Map Sizes
    unsigned int layers;
    sf::Vector2i v_gridSize;
    sf::Vector2i v_tileSize;
    sf::Vector2f scaleFactor;

    // Starting positions
    sf::Vector2f playerStartingPosition;
    std::vector<MapObject> enemyData;
    std::vector<MapObject> npcData;
    std::vector<MapObject> leverData;
    std::vector<MapObject> doorData;
    std::vector<MapObject> coinData;
    std::vector<MapObject> toolData;
    std::vector<MapObject> exitData;
    std::map<std::string, std::string> mapMetadata;

    std::vector<std::vector<std::vector<sf::Sprite*>>> map;
    sf::RectangleShape* background;
    std::vector<Hitbox*> v_mapHitboxes;
public:
    Map(std::string filename, sf::Vector2i overrideTileSize, const int& entranceIndex);
    ~Map();

    sf::Vector2f getMapTotalPixelSize();

    // Starting position getters
    sf::Vector2f getPlayerPosition();
    std::vector<MapObject> getEnemyData();
    std::vector<MapObject> getNPCData();
    std::vector<MapObject> getLeverData();
    std::vector<MapObject> getDoorData();
    std::vector<MapObject> getCoinData();
    std::vector<MapObject> getToolData();
    std::vector<MapObject> getExitData();
    std::string getMetadataValue(std::string key);

    void render();
    void renderBackground();
    void renderFront();
};

#endif
