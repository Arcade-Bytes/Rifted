#include "SummaryState.h"


SummaryState::SummaryState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = SUMMARY_STATE;

    ResourceManager* resource = ResourceManager::getInstance();
    sf::Vector2f baseResolution = sf::Vector2f(Engine::getInstance()->getBaseResolution().x,Engine::getInstance()->getBaseResolution().y);

    this->bgShape.setTexture(resource->loadTexture("resources/TheRift.jpg"));
    this->bgShape.setSize(baseResolution);
    this->scrollShape.setTexture(resource->loadTexture("resources/parchment.png"));
    this->scrollShape.setSize(sf::Vector2f(baseResolution.x*1.5f, baseResolution.y*1.75f));
    this->scrollShape.setPosition(sf::Vector2f(-baseResolution.x*0.1f,-baseResolution.y*0.4f));

    // Texts init
    for(int i=0; i<4; i++)
    {
        this->v_texts.push_back(sf::Text());
        this->v_texts[i].setFont(*resource->loadFont("resources/XOX.ttf"));
        this->v_texts[i].setCharacterSize(baseResolution.x*0.04f);
        this->v_texts[i].setColor(sf::Color::Black);
        this->v_texts[i].setPosition(sf::Vector2f(
            baseResolution.x*0.45f,
            baseResolution.y*0.43 + i*baseResolution.y*0.12
        ));
    }

    // Weapon upgrade data
    std::string textureNames[4] = {"health_upgrade","sword_upgrade","hammer","bow"};
    for(int i=0; i<4; i++)
    {
        this->v_sprites.push_back(sf::RectangleShape());
        this->v_sprites[i].setTexture(resource->loadTexture("resources/"+textureNames[i]+".png"));
        this->v_sprites[i].setSize(sf::Vector2f(baseResolution.x*0.1f,baseResolution.y*0.1f));
        this->v_sprites[i].setPosition(sf::Vector2f(baseResolution.x*(0.4f+i*0.15f),baseResolution.y*0.25f));

        this->v_texts.push_back(sf::Text());
        int j = v_texts.size()-1;
        this->v_texts[j].setFont(*resource->loadFont("resources/XOX.ttf"));
        this->v_texts[j].setCharacterSize(baseResolution.x*0.018f);
        this->v_texts[j].setColor(sf::Color::Black);
        this->v_texts[j].setPosition(sf::Vector2f(
            baseResolution.x*(0.38f+i*0.15f),
            baseResolution.y*0.37f
        ));
    }
    // Upgrade title
    this->v_texts.push_back(sf::Text());
    int x = this->v_texts.size()-1;
    this->v_texts[x].setString("Purchased upgrades:");
    this->v_texts[x].setFont(*resource->loadFont("resources/XOX.ttf"));
    this->v_texts[x].setCharacterSize(baseResolution.x*0.04f);
    this->v_texts[x].setColor(sf::Color::Black);
    this->v_texts[x].setPosition(sf::Vector2f(
        baseResolution.x*0.4f,
        baseResolution.y*0.15f
    ));

    // Thanks for playing texts
    this->v_texts.push_back(sf::Text());
    x = this->v_texts.size()-1;
    this->v_texts[x].setString("Rifted");
    this->v_texts[x].setFont(*resource->loadFont("resources/XOX.ttf"));
    this->v_texts[x].setCharacterSize(baseResolution.x*0.05f);
    this->v_texts[x].setColor(sf::Color::White);
    this->v_texts[x].setPosition(sf::Vector2f(
        baseResolution.x*0.04f,
        baseResolution.y*0.1f
    ));
    this->v_texts.push_back(sf::Text());
    x = this->v_texts.size()-1;
    this->v_texts[x].setString("Thanks for playing!");
    this->v_texts[x].setFont(*resource->loadFont("resources/XOX.ttf"));
    this->v_texts[x].setCharacterSize(baseResolution.x*0.02f);
    this->v_texts[x].setColor(sf::Color::White);
    this->v_texts[x].setPosition(sf::Vector2f(
        baseResolution.x*0.027f,
        baseResolution.y*0.2f
    ));
    this->v_texts.push_back(sf::Text());
    x = this->v_texts.size()-1;
    this->v_texts[x].setString("Press Enter or \nEscape to continue");
    this->v_texts[x].setFont(*resource->loadFont("resources/XOX.ttf"));
    this->v_texts[x].setCharacterSize(baseResolution.x*0.02f);
    this->v_texts[x].setColor(sf::Color::White);
    this->v_texts[x].setPosition(sf::Vector2f(
        baseResolution.x*0.03f,
        baseResolution.y*0.3f
    ));
}
SummaryState::~SummaryState()
{
    v_sprites.clear();
    v_texts.clear();
}

void SummaryState::initState()
{
    this->b_reInit = false;

    // Data gathering
    int points = this->player->getPoints();
    int kills = atoi(this->player->getKills().c_str());
    int coins = atoi(this->player->getTotalCoins().c_str());
    int maxCoins = 96; // 24 every tool max upgrade * 4 upgradable tools

    int upgradableEquipment = 4;
    int upgrades[upgradableEquipment];
    upgrades[0] = atoi(this->player->getHealthUpg().c_str());
    upgrades[1] = atoi(this->player->getSwordLvl().c_str());
    upgrades[2] = atoi(this->player->getHammrLvl().c_str());
    upgrades[3] = atoi(this->player->getBowLvl().c_str());

    int totalPoints = points;
    // 1000 extra pts for every purchased upgrade
    for(int i=0; i<4; i++) totalPoints += upgrades[i]*1000;
    // 5000 pts for beating the game
    totalPoints += 5000;

    // Text setup
    this->v_texts[0].setString("Base points: "+std::to_string(points));
    this->v_texts[1].setString("Total kills: "+std::to_string(kills));
    this->v_texts[2].setString("Coins collected: "+std::to_string(coins)+" / "+std::to_string(maxCoins));
    this->v_texts[3].setString("Total points: "+std::to_string(totalPoints));
    // Upgrade texts
    std::string upgradeType[4] = {"Life: ","Sword: ","Hammer: ","Bow: "};
    for(int i=0; i<4; i++)
    {
        if(upgrades[i] >= 0)
        {
            this->v_texts[i+4].setString(upgradeType[i]+std::to_string(upgrades[i])+"/3");
            this->v_sprites[i].setFillColor(sf::Color::White);
        }
        else
        {
            this->v_texts[i+4].setString(upgradeType[i]+"-");
            this->v_sprites[i].setFillColor(sf::Color(50,50,50));
        }
    }
}

void SummaryState::update()
{
    // Check if we need to update our data
    if(this->b_reInit) this->initState();

    if(Engine::getInstance()->getKeyPressed(sf::Keyboard::Escape) || Engine::getInstance()->getKeyPressed(sf::Keyboard::Return))
    {
        this->changeState(CREDITS_STATE, true);
    }
}

void SummaryState::render(float frameProgress)
{
    if(this->b_reInit) return;
    Engine* engine = Engine::getInstance();
    engine->setFollowView(false);

    engine->renderDrawable(&bgShape);
    engine->renderDrawable(&scrollShape);
    for(auto &text : v_texts)
        engine->renderDrawable(&text);
    for(auto &shape : v_sprites)
        engine->renderDrawable(&shape);
}
