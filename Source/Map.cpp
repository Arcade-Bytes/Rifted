#include <unistd.h>
#include <limits.h>
#include <iostream>
#include "Map.h"

struct MapObject;

Map::Map(std::string filename, sf::Vector2i overrideTileSize, const int& entranceIndex)
{
    // Init map data
    XMLDocument doc;
    std::string filenameComplete = "maps/"+filename+".tmx";
    if(doc.LoadFile(filenameComplete.c_str())!=0)
    {
        filenameComplete = "../maps/"+filename+".tmx";
        doc.LoadFile(filenameComplete.c_str());
    }
    XMLElement * mapdata = doc.FirstChildElement("map");
    mapdata->QueryIntAttribute("width", &this->v_gridSize.x);
    mapdata->QueryIntAttribute("height", &this->v_gridSize.y);
    mapdata->QueryIntAttribute("tilewidth", &this->v_tileSize.x);
    mapdata->QueryIntAttribute("tileheight", &this->v_tileSize.y);
    v_tileSize = overrideTileSize;

    // Layer count
    this->layers = 0;
    XMLElement* layer = mapdata->FirstChildElement("layer");
    while(layer)
    {
        this->layers++;
        layer = layer->NextSiblingElement("layer");
    }

    // Tileset Data
    XMLElement* tileset = mapdata->FirstChildElement("tileset");
    XMLElement* image = tileset->FirstChildElement("image");
    int imgWidth, imgHeight;
    const char *textureFich;
    tileset->QueryIntAttribute("tilewidth", &this->v_tilesetTileSize.x);
    tileset->QueryIntAttribute("tileheight", &this->v_tilesetTileSize.y);
    image->QueryIntAttribute("width", &imgWidth);
    image->QueryIntAttribute("height", &imgHeight);
    textureFich = image->Attribute("source");
    std::string textureFileName = std::string(textureFich);

    this->v_tilesetGridSize.x = imgWidth / this->v_tilesetTileSize.x;
    this->v_tilesetGridSize.y = imgHeight / this->v_tilesetTileSize.y;
    this->i_tilesetLinearSize = v_tilesetGridSize.x * v_tilesetGridSize.y;

    this->scaleFactor = {
        v_tileSize.x / (float)v_tilesetTileSize.x,
        v_tileSize.y / (float)v_tilesetTileSize.y
    };

    // Background data
    tileset = tileset->NextSiblingElement("tileset");
    if(tileset)
    {
        image = tileset->FirstChildElement("image");
        textureFich = image->Attribute("source");
        this->background = new sf::RectangleShape();
        this->background->setTexture(ResourceManager::getInstance()->loadTexture("maps/"+std::string(textureFich)));
        this->background->setPosition(0,0);
        this->background->setSize(sf::Vector2f(v_tileSize.x*v_gridSize.x,v_tileSize.y*v_gridSize.y));
    }
    else
    {
        this->background = NULL;
    }
    

    // Map reading
    layer = mapdata->FirstChildElement("layer");
    XMLElement* tile = NULL;
    this->map.resize(this->layers);
    for (unsigned int l = 0; l < this->layers && layer; l++)
    {
        // First tile pointer
        tile = layer->FirstChildElement("data")->FirstChildElement("tile");
        this->map[l].resize(this->v_gridSize.x);
        for (int y = 0; y < this->v_gridSize.y && tile; y++)
        {
            for (int x = 0; x < this->v_gridSize.x && tile; x++)
            {
                // Init vector on first tile
                if(y == 0) this->map[l][x].resize(this->v_gridSize.y);

                // Create tile
                int gid=0; tile->QueryIntAttribute("gid", &gid);
                if(gid > 0)
                {
                    gid--;
                    int coordX = (gid) % this->v_tilesetGridSize.x;
                    int coordY = (gid) / this->v_tilesetGridSize.x;
                    this->map[l][x][y] = new sf::Sprite(*ResourceManager::getInstance()->loadTexture("maps/"+textureFileName));
                    this->map[l][x][y]->setTextureRect(sf::IntRect(
                        coordX * this->v_tilesetTileSize.x,
                        coordY * this->v_tilesetTileSize.y,
                        this->v_tilesetTileSize.x,
                        this->v_tilesetTileSize.y
                    ));
                    this->map[l][x][y]->setScale(this->scaleFactor);
                    this->map[l][x][y]->setPosition(x * this->v_tileSize.x, y * this->v_tileSize.y);
                }
                else
                {
                    this->map[l][x][y] = NULL;
                }

                // Next tile
                tile = tile->NextSiblingElement("tile");
            }
        }
        // Next layer
        layer = layer->NextSiblingElement("layer");
    }

    // Load entities
    XMLElement* group = mapdata->FirstChildElement("objectgroup");
    XMLElement* object;
    while(group)
    {
        const char* name = group->Attribute("name");
        object = group->FirstChildElement("object");
        int entranceCounter = 0;
        while(object)
        {
            sf::Vector2f position;
            sf::Vector2f size;
            object->QueryFloatAttribute("x", &position.x);
            object->QueryFloatAttribute("y", &position.y);
            object->QueryFloatAttribute("width", &size.x);
            object->QueryFloatAttribute("height", &size.y);
            position.x *= this->scaleFactor.x;
            position.y *= this->scaleFactor.y;
            size.x *= this->scaleFactor.x;
            size.y *= this->scaleFactor.y;
            position.x += size.x/2;
            position.y += size.y/2;
            // The player
            if(strcmp(name, "Entradas") == 0)
            {
                if(entranceCounter == entranceIndex)
                {
                    this->playerStartingPosition = position;
                }
                entranceCounter++;
            }
            else if(strcmp(name, "Salidas") == 0)
            {
                const char* objName = object->Attribute("name");
                MapObject mapObject;
                mapObject.positon = position;
                mapObject.size = size;
                mapObject.name = std::string(objName);
                this->exitData.push_back(mapObject);
            }
            else if(strcmp(name, "NPC") == 0)
            {
                const char* objName = object->Attribute("name");
                MapObject mapObject;
                mapObject.positon = position;
                mapObject.size = size;
                mapObject.name = std::string(objName);
                this->npcData.push_back(mapObject);
            }
            else if(strcmp(name, "Monedas") == 0)
            {
                MapObject mapObject;
                mapObject.positon = position;
                mapObject.size = size;
                this->coinData.push_back(mapObject);
            }
            else if(strcmp(name, "Herramientas") == 0)
            {
                const char* objName = object->Attribute("name");
                MapObject mapObject;
                mapObject.positon = position;
                mapObject.size = size;
                mapObject.name = std::string(objName);
                this->toolData.push_back(mapObject);
            }
            else if(strcmp(name, "Hitboxes") == 0)
            {
                Hitbox* hitbox = new Hitbox(PLATFORM, size.x, size.y, position.x, position.y);
                this->v_mapHitboxes.push_back(hitbox);
            }
            else if(strcmp(name, "ZonasLetales") == 0)
            {
                Hitbox* hitbox = new Hitbox(LETHAL, size.x, size.y, position.x, position.y, 1000);
                this->v_mapHitboxes.push_back(hitbox);
            }
            else if(strcmp(name, "Palancas") == 0)
            {
                const char* objName = object->Attribute("name");
                MapObject mapObject;
                mapObject.positon = position;
                mapObject.size = size;
                mapObject.name = std::string(objName);
                this->leverData.push_back(mapObject);
            }
            else if(strcmp(name, "Puertas") == 0)
            {
                const char* objName = object->Attribute("name");
                MapObject mapObject;
                mapObject.positon = position;
                mapObject.size = size;
                mapObject.name = std::string(objName);
                this->doorData.push_back(mapObject);
            }
            else
            {
                MapObject mapObject;
                mapObject.positon = position;
                mapObject.size = size;
                mapObject.type = atoi(name);
                this->enemyData.push_back(mapObject);
            }

            // Next object
            object = object->NextSiblingElement("object");
        }
        // Next group
        group = group->NextSiblingElement("objectgroup");
    }
}

Map::~Map()
{
    if(this->background) delete this->background;
    for (unsigned int l = 0; l < this->layers; l++)
    {
        for (int x = 0; x < this->v_gridSize.x; x++)
        {
            for (int y = 0; y < this->v_gridSize.y; y++)
            {
                delete this->map[l][x][y];
                this->map[l][x][y] = NULL;
            }
            map[l][x].clear();
        }
        map[l].clear();
    }
    map.clear();

    for(auto hitbox : v_mapHitboxes)
    {
        delete hitbox;
        hitbox = NULL;
    }
    v_mapHitboxes.clear();
}

sf::Vector2f Map::getPlayerPosition()
{
    return this->playerStartingPosition;
}

std::vector<MapObject> Map::getEnemyData()
{
    return this->enemyData;
}

std::vector<MapObject> Map::getNPCData()
{
    return this->npcData;
}

std::vector<MapObject> Map::getLeverData()
{
    return this->leverData;
}

std::vector<MapObject> Map::getDoorData()
{
    return this->doorData;
}

std::vector<MapObject> Map::getCoinData()
{
    return this->coinData;
}

std::vector<MapObject> Map::getToolData()
{
    return this->toolData;
}

std::vector<MapObject> Map::getExitData()
{
    return this->exitData;
}

void Map::render()
{
    Engine* engine = Engine::getInstance();

    // Background
    if(this->background) engine->renderDrawable(this->background);

    // Tile map
    for (unsigned int l = 0; l < this->layers; l++)
        for (int x = 0; x < this->v_gridSize.x; x++)
            for (int y = 0; y < this->v_gridSize.y; y++)
                if(this->map[l][x][y])
                    engine->renderDrawable(this->map[l][x][y]);

    // Debug Hitbox
    //for(auto hitbox : v_mapHitboxes)
    //    hitbox->render();
}
