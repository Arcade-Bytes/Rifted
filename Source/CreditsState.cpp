#include "CreditsState.h"


CreditsState::CreditsState(std::stack<State*>* states, Player* player)
    :State(states, player)
{
    this->Iam = CREDITS_STATE;


    ResourceManager* resource = ResourceManager::getInstance();
    sf::Vector2f baseResolution = sf::Vector2f(Engine::getInstance()->getBaseResolution().x,Engine::getInstance()->getBaseResolution().y);

    this->bgShape.setTexture(resource->loadTexture("resources/TheRift.jpg"));
    this->bgShape.setSize(baseResolution);

    float baseHeight = baseResolution.y*0.6f;
    float separation = baseResolution.y*0.2;

    this->f_creditsTime = 20.0f;
    this->f_creditsSize = baseHeight;

    // Texts init
    for(int i=0; i<9; i++)
    {
        this->v_texts.push_back(sf::Text());
        this->v_texts[i].setFont(*resource->loadFont("resources/XOX.ttf"));
        this->v_texts[i].setCharacterSize(baseResolution.x*0.04f);
        this->v_texts[i].setColor(sf::Color::White);
        this->v_texts[i].setPosition(sf::Vector2f(
            baseResolution.x*0.1f,
            baseHeight + i*separation
        ));
        this->f_creditsSize += separation;
    }
    this->f_creditsSize += baseResolution.y;

    // Text string init
    int i=0;
    this->v_texts[i++].setString("Rifted");
    this->v_texts[i++].setString("Alex Macia Fiteni\n   - Morwen y Dora");
    this->v_texts[i++].setString("Borja Martin Rodriguez\n   - Seven Eleven");
    this->v_texts[i++].setString("Ivan Pomares Rastrollo\n   - Kriegger (?)");
    this->v_texts[i++].setString("Jose Miguel Brotons Valero\n   - Vendrick");
    this->v_texts[i++].setString("Lucas Mataix Garrigos\n   - Abascal Semental");
    this->v_texts[i++].setString("\n\nSiempre te recordaremos, Orianna");
    this->v_texts[i++].setString("\nY que Grimaldus guie tu camino");
    this->v_texts[i++].setString("\n\n\nBy Arcade Bytes");

    // Extra, first text is special (the Title)
    this->v_texts[0].setCharacterSize(baseResolution.x*0.1f);
    this->v_texts[0].setPosition(sf::Vector2f(
        baseResolution.x*0.3f,
        0
    ));
}
CreditsState::~CreditsState()
{
    v_texts.clear();
}

void CreditsState::initState()
{
    this->b_reInit = false;

    // Reset time counter
    this->f_timeCounter = 0.0f;

    // Reset view position
    float xPos = Engine::getInstance()->getBaseResolution().x;
    xPos /= 2.0f;
    this->vf_nextPosition = this->vf_prevPosition = {
        xPos,
        0
    };
}

void CreditsState::update()
{
    // Check if we need to update our data
    if(this->b_reInit) this->initState();

    this->f_timeCounter += Engine::getInstance()->getDelta();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        this->f_timeCounter += Engine::getInstance()->getDelta()*2;

    float progress = this->f_timeCounter / this->f_creditsTime;

    this->vf_prevPosition = this->vf_nextPosition;
    this->vf_nextPosition.y = progress * this->f_creditsSize;

    if(Engine::getInstance()->getKeyPressed(sf::Keyboard::Escape) || this->f_timeCounter >= this->f_creditsTime)
    {
        this->changeState(MENU_STATE, true);
    }
}

void CreditsState::render(float frameProgress)
{
    if(this->b_reInit) return;
    Engine* engine = Engine::getInstance();

    // Render background
    engine->setFollowView(false);
    engine->renderDrawable(&bgShape);

    // Render names
    engine->setFollowView(true);
    sf::Vector2f pos = this->vf_prevPosition + (this->vf_nextPosition - this->vf_prevPosition) * frameProgress;
    engine->setViewCenter(pos);
    for(auto &text : v_texts)
        engine->renderDrawable(&text);
}
