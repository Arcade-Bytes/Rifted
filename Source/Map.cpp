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
    this->v_totalPixelSize = {
        this->v_tileSize.x * (float)this->v_gridSize.x,
        this->v_tileSize.y * (float)this->v_gridSize.y
    };
    this->b_scrollingBackground = false;

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
                if(entranceCounter==0) this->playerStartingPosition = position;
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
                mapObject.type = 0;
                this->doorData.push_back(mapObject);
            }
            else if(strcmp(name, "PuertasH") == 0)
            {
                const char* objName = object->Attribute("name");
                MapObject mapObject;
                mapObject.positon = position;
                mapObject.size = size;
                mapObject.name = std::string(objName);
                mapObject.type = 1;
                this->doorData.push_back(mapObject);
            }
            else if(strcmp(name, "Metadata") == 0)
            {
                std::string objName = std::string(object->Attribute("name"));
                int splitter = objName.find("/");
                std::string key = objName.substr(0,splitter);
                std::string value = objName.substr(splitter+1);

                this->mapMetadata[key] = value;
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

    // Scrolling background data
    if(this->getMetadataValue("bgScroll") != "")
    {
        sf::Vector2f baseRes = {(float)Engine::getInstance()->getReferenceResolution().x,(float)Engine::getInstance()->getReferenceResolution().y};
        float scale = v_totalPixelSize.x/baseRes.x;

        if(scale > 1.0f)
        {
            this->b_scrollingBackground = true;
            this->f_bgScrollScale = scale > 1.5f ? 1.5f : 1.0f;

            this->background->setSize(baseRes * f_bgScrollScale);
        }
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
    this->map.clear();

    for(auto hitbox : v_mapHitboxes)
    {
        delete hitbox;
        hitbox = NULL;
    }
    this->v_mapHitboxes.clear();
    this->mapMetadata.clear();
}

sf::Vector2f Map::getMapTotalPixelSize()
{
    return this->v_totalPixelSize;
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

std::string Map::getMetadataValue(std::string key)
{
    if(mapMetadata.find(key) != mapMetadata.end())
    {
        return mapMetadata[key];
    }
    else
    {
        return "";
    }
}

void Map::render()
{
    // Background and Front
    this->renderBackground();
    this->renderBack();
    this->renderFront();

    // Debug Hitbox
    //for(auto hitbox : v_mapHitboxes)
    //    hitbox->render();
}

void Map::renderBackground(sf::Vector2f playerPos)
{
    Engine* engine = Engine::getInstance();

    // Move the background if we're scrolling
    if(this->b_scrollingBackground)
    {
        float screenSizeX = engine->getReferenceResolution().x;
        float progressX = (playerPos.x - screenSizeX/2.0f) / (this->v_totalPixelSize.x - screenSizeX);
        if(progressX < 0.0f) progressX = 0.0f;
        if(progressX > 1.0f) progressX = 1.0f;
        float xPoint = progressX * (this->v_totalPixelSize.x - screenSizeX*f_bgScrollScale);

        float screenSizeY = engine->getReferenceResolution().y;
        float progressY = (playerPos.y - screenSizeY/2.0f) / (this->v_totalPixelSize.y - screenSizeY);
        if(progressY < 0.0f) progressY = 0.0f;
        if(progressY > 1.0f) progressY = 1.0f;
        float yPoint = progressY * (this->v_totalPixelSize.y - screenSizeY*f_bgScrollScale);

        this->background->setPosition(xPoint, yPoint);
    }

    // Background
    if(this->background) engine->renderDrawable(this->background);
}

void Map::renderBack()
{
    Engine* engine = Engine::getInstance();

    // Layer count
    unsigned int layersTmp = this->layers;
    if(this->layers > 1) layersTmp--;

    // Tile map
    for (unsigned int l = 0; l < layersTmp; l++)
        for (int x = 0; x < this->v_gridSize.x; x++)
            for (int y = 0; y < this->v_gridSize.y; y++)
                if(this->map[l][x][y])
                    engine->renderDrawable(this->map[l][x][y]);
}

void Map::renderFront()
{
    Engine* engine = Engine::getInstance();

    // Layer count
    if(this->layers > 1)
    {
        int l=this->layers-1;
        for (int x = 0; x < this->v_gridSize.x; x++)
            for (int y = 0; y < this->v_gridSize.y; y++)
                if(this->map[l][x][y])
                    engine->renderDrawable(this->map[l][x][y]);
    }        
}
