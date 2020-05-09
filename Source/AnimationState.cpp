#include "AnimationState.h"


AnimationState::AnimationState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = ANIMATION_STATE;

    this->baseResolution = sf::Vector2f(Engine::getInstance()->getBaseResolution().x,Engine::getInstance()->getBaseResolution().y);

    this->bgShape = new sf::RectangleShape();
    this->bgShape->setSize(this->baseResolution);
}
AnimationState::~AnimationState()
{
    delete this->bgShape;
    this->clearState();
}

void AnimationState::clearState()
{
    for(auto ent : v_entities)
    {
        delete ent;
        ent = NULL;
    }
    v_entities.clear();
}

void AnimationState::initState()
{
    this->b_reInit = false;
    this->clearState();
    float floatPrecision = 100.0f;

    this->f_timeCounter = 0.0f;
    this->f_animationDuration = 10.0f;

    // Recover animation data file
    std::string filename = this->player->getAnimationFilename();
    filename = "animations/complex/" + filename + ".json";

    // Open and parse
    FILE* filecontent = fopen(filename.c_str(), "rb");
    if(!filecontent) return;
    char buffer[65536];
    rapidjson::FileReadStream is(filecontent, buffer, sizeof(buffer));
    rapidjson::Document document;
    document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);

    // Recover the background image file name
    std::string backgroundFile = "GrayScale.png";
    if(document.HasMember("background"))
        backgroundFile = document["background"].GetString();
    this->bgShape->setTexture(ResourceManager::getInstance()->loadTexture("resources/"+backgroundFile));

    // Recover the music data
    std::string musicFile = "";
    if(document.HasMember("music"))
        musicFile = document["music"].GetString();

    // Recover the animation time
    if(document.HasMember("animationTime"))
        this->f_animationDuration = document["animationTime"].GetInt() / floatPrecision;

    // Load animated enitity data
    rapidjson::Value& entityData = document["entities"];
    for(unsigned int i = 0; i < entityData.Size() ; i++)
    {
        // Load basic data
        std::string animationJson = "";
        if(entityData[i].HasMember("animationFileCondition"))
        {
            std::string condition = entityData[i]["animationFileCondition"].GetString();
            int candidates = entityData[i]["animationFile"].Size();
            int index = this->getGameValue(condition, candidates);
            animationJson = entityData[i]["animationFile"][index].GetString();
        }
        else
        {
            animationJson = entityData[i]["animationFile"].GetString();
        }
        sf::Vector2f start = {
            entityData[i]["startX"].GetInt() / floatPrecision,
            entityData[i]["startY"].GetInt() / floatPrecision
        };
        start /= 100.0f;
        start.x *= baseResolution.x;
        start.y *= baseResolution.y;
        float percSize = entityData[i]["size"].GetInt() / floatPrecision;
        percSize /= 100.0f;
        percSize *= baseResolution.x;
        sf::Vector2f size = {percSize, percSize};

        // Add the new entity
        AnimatedEntity* newEntity = new AnimatedEntity(start, size, animationJson);

        // Load actions
        rapidjson::Value& actions = entityData[i]["actions"];
        for(unsigned int j = 0; j < actions.Size() ; j++)
        {
            float duration = actions[j]["duration"].GetInt() / 1000.0f; // ms to s
            float nextX = actions[j]["moveX"].GetInt() / floatPrecision;
            float nextY = actions[j]["moveY"].GetInt() / floatPrecision;
            float percSize = actions[j]["size"].GetInt() / floatPrecision;
            std::string animationName = actions[j]["animation"].GetString();
            bool mirrored = actions[j]["mirror"].GetBool();

            // Adjust data
            nextX /= 100.0f;
            nextY /= 100.0f;
            nextX *= baseResolution.x;
            nextY *= baseResolution.y;
            percSize /= 100.0f;
            percSize *= baseResolution.x;
            sf::Vector2f size = {percSize, percSize};

            // Add the new action
            newEntity->addAction(
                duration, sf::Vector2f(nextX, nextY), size,
                animationName, mirrored
            );
        }

        // Add the new entity to the main list
        this->v_entities.push_back(newEntity);
    }

    fclose(filecontent);

    // Music start
    if(musicFile != "")
        ResourceManager::getInstance()->playLevelMusic(musicFile);
}

int AnimationState::getGameValue(std::string property, int nCandidates)
{
    int result = 0;

    // Recovered boss key data
    if(property == "bossKeyIndex")
    {
        result = this->player->getRecoveredKey();
    }

    // Clamp results
    if(result < 0 || result >= nCandidates)
        result = 0;

    return result;
}

void AnimationState::update()
{
    // Check if we need to update our data
    if(this->b_reInit) this->initState();

    for(auto entity : v_entities)
        entity->update();

    // End update
    this->f_timeCounter += Engine::getInstance()->getDelta();
    if(Engine::getInstance()->getKeyPressed(sf::Keyboard::Escape) || Engine::getInstance()->getKeyPressed(sf::Keyboard::Return))
    {
        this->f_timeCounter = this->f_animationDuration;
    }
    if(this->f_timeCounter >= this->f_animationDuration)
    {
        this->changeState(GAME_STATE, true);
    }
}

void AnimationState::render(float frameProgress)
{
    if(this->b_reInit) return;
    Engine* engine = Engine::getInstance();
    engine->setFollowView(false);

    engine->renderDrawable(bgShape);
    for(auto entity : v_entities)
        entity->render(frameProgress);
}
