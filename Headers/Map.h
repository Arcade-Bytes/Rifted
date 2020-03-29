#ifndef MAP_H
#define MAP_H

#include "Engine.h"
#include "../tinyxml2-master/tinyxml2.h"

using namespace tinyxml2;

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
    std::vector<sf::Vector2f> enemiesStartingPositions;
    std::vector<sf::Vector2f> enemiesSizes;
    std::vector<int> enemiesTypes;

    std::vector<std::vector<std::vector<sf::Sprite*>>> map;
public:
    Map(const char* filename);
    ~Map();

    // Starting position getters
    sf::Vector2f getPlayerPosition();
    std::vector<sf::Vector2f> getEnemyPositions();
    std::vector<sf::Vector2f> getEnemySizes();
    std::vector<int> getEnemyTypes();

    void render();
};

#endif
