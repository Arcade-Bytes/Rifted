#include "Map.h"

Map::Map(const char* filename)
{
    // Init map data
    XMLDocument doc;
    doc.LoadFile(filename);
    XMLElement * mapdata = doc.FirstChildElement("map");
    mapdata->QueryIntAttribute("width", &this->v_gridSize.x);
    mapdata->QueryIntAttribute("height", &this->v_gridSize.y);
    mapdata->QueryIntAttribute("tilewidth", &this->v_tileSize.x);
    mapdata->QueryIntAttribute("tileheight", &this->v_tileSize.y);
    v_tileSize = {10,10};

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

    this->tilesetTexture = new sf::Texture();
    this->tilesetTexture->loadFromFile("maps/"+std::string(textureFich));
    this->v_tilesetGridSize.x = imgWidth / this->v_tilesetTileSize.x;
    this->v_tilesetGridSize.y = imgHeight / this->v_tilesetTileSize.y;
    this->i_tilesetLinearSize = v_tilesetGridSize.x * v_tilesetGridSize.y;

    this->scaleFactor = {
        v_tileSize.x / (float)v_tilesetTileSize.x,
        v_tileSize.y / (float)v_tilesetTileSize.y
    };

    printf("Estos son los datos: width: %i  height: %i tilewidth: %i tileheigth: %i fichero tileset: %s \n",
        this->v_gridSize.x, this->v_gridSize.y, this->v_tileSize.x, this->v_tileSize.y, textureFich);
    printf("Estos son más datos: tilesetTilewidth: %i  tilesetTileheight: %i tilesetGridwidth: %i  tilesetGridheight: %i \n",
        this->v_tilesetTileSize.x, this->v_tilesetTileSize.y, this->v_tilesetGridSize.x, this->v_tilesetGridSize.y);

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
                int gid; tile->QueryIntAttribute("gid", &gid);
                if(gid > 0)
                {
                    gid--;
                    int coordX = (gid) % 100;
                    int coordY = (gid) / 100;
                    this->map[l][x][y] = new sf::Sprite(*tilesetTexture);
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
            if(strcmp(name, "Personaje") == 0)
            {
                this->playerStartingPosition = position;
            }
            else
            {
                this->enemiesStartingPositions.push_back(position);
                this->enemiesSizes.push_back(size);
                this->enemiesTypes.push_back(atoi(name));
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
    delete this->bgTexture;
    delete this->tilesetTexture;
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
}

sf::Vector2f Map::getPlayerPosition()
{
    return this->playerStartingPosition;
}

std::vector<sf::Vector2f> Map::getEnemyPositions()
{
    return this->enemiesStartingPositions;
}

std::vector<sf::Vector2f> Map::getEnemySizes()
{
    return this->enemiesSizes;
}

std::vector<int> Map::getEnemyTypes()
{
    return this->enemiesTypes;
}

void Map::render()
{
    Engine* engine = Engine::getInstance();
    for (unsigned int l = 0; l < this->layers; l++)
        for (int x = 0; x < this->v_gridSize.x; x++)
            for (int y = 0; y < this->v_gridSize.y; y++)
                if(this->map[l][x][y])
                    engine->renderDrawable(this->map[l][x][y]);
}
