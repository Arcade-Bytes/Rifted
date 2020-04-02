#include "Player.h"
#include "FileManager.h"

Player::Player()
    : Entity()
{
    this->sword = new Weapon(0.3f, 0.1f, 40, 60);
    this->hammer = new Weapon(1.0f, 0.8f, 60, 70);
    this->shield = new Shield(0.2f, 0.2f, 0.05f, 0.02f);
    this->bow = new Weapon(0.3f, 0.1f, 40, 60); //testing grounds, melee bow cha cha cha
    ftl::LoadGame(*this);
}

Player::~Player()
{
    delete this->sword;
    delete this->hammer;
    delete this->shield;
}

void Player::getHurt(float& damage)
{
    damage *= this->shield->DamageBlock();
    f_currentHealth -= damage;
    if(f_currentHealth <= 0.0f)
    {
        //f_currentHealth = 0.001f;
    }
    f_regenerationDelta = 0.0f;
}

void Player::getHealed(float& healing)
{
    f_currentHealth += healing;
    if(f_currentHealth >= f_maxHealth)
    {
        f_currentHealth = f_maxHealth;
    }
    f_regenerationDelta = 0.0f;
}

void Player::regenerate()
{
    float delta = Engine::getInstance()->getDelta();

    // Get current life chunk
    float chunkSize = f_maxHealth / i_nchunks;
    int i_currentChunk = ceil(f_maxHealth / chunkSize);

    f_regenerationDelta += delta;
    if(f_regenerationDelta >= f_regenerationCooldown && f_currentHealth < i_currentChunk*chunkSize)
    {
        // Regenerate life
        f_currentHealth += f_regenerationAmount;
        // Wait for (f_regenerationSpeed) seconds until next regen
        f_regenerationDelta = f_regenerationCooldown - f_regenerationSpeed;
        // Cap the life regen to avoid skipping chunks
        if(f_currentHealth >= i_currentChunk * chunkSize)
            f_currentHealth = i_currentChunk * chunkSize;
    }
}

void Player::update()
{
    // Update input
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        this->move(1);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        this->move(-1);
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
    sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
        this->jump(50);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::G))
        this->sword->startAttack();
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        this->hammer->startAttack();
    /***XML TEST***/
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
    {
        ftl::SaveGame(*this);
       
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::F7))
    {
        ftl::LoadGame(*this);
    }
    /*****/
    // Update the attacks
    this->updateWeapon(sword);
    this->updateWeapon(hammer);

    // Update movement state
    this->updateMovement();

    // Update shield state
    this->shield->setPosition(this->vf_position.x, this->vf_position.y, true);
    this->shield->update();

    // Update life regeneration
    this->regenerate();
}

void Player::render()
{
    Engine::getInstance()->renderDrawable(&shape);
    this->sword->render();
    this->hammer->render();
    this->shield->render();
    this->hitbox->render();
<<<<<<< HEAD
}

//GET DATA TO SAVE
std::string Player::getDeaths()
{
    return std::to_string(this->i_deaths).c_str();
}

std::string Player::getKills()
{
    return std::to_string(this->i_kills).c_str();
}

std::string Player::getMony()
{
    return std::to_string(this->i_coins).c_str();
}

std::string Player::getLevel()
{
    return this->s_levelName.c_str();
}

std::string Player::getDoor()
{
    return std::to_string(this->i_door).c_str();
}

std::string Player::getHealthUpg()
{
    return std::to_string(this->i_healthUpg).c_str();
}

std::string Player::getHammrLvl()
{
    return std::to_string(this->hammer->getUpgradeLvl()).c_str();
}

std::string Player::getSwordLvl()
{
    return std::to_string(this->sword->getUpgradeLvl()).c_str();
}

std::string Player::getShieldLvl()
{
    return std::to_string(this->shield->getUpgradeLvl()).c_str();
}

std::string Player::getBowLvl()
{
    return std::to_string(this->bow->getUpgradeLvl()).c_str();
=======
>>>>>>> d0f482223e1cd0fa998853caf699f2ef19fe23ae
}
//SET DATA SAVED

    void Player::setMony(int i_money)
    {
        this->i_coins = i_money;
    }
    
    void Player::setKills(int i_kills)
    {
        this->i_kills = i_kills;
    }
    
    void Player::setDeaths(int i_deaths)
    {
        this->i_deaths = i_deaths;
    }
    
    void Player::setHealthUpg(int i_healthupg)
    {
        this->i_healthUpg = i_healthupg;
    }
    
    void Player::setLevel(std::string s_levelName)
    {
        this->s_levelName = s_levelName;
    }
    
    void Player::setDoor(int i_door)
    {
        this->i_door = i_door;
    }
    
    void Player::setHammrLvl(int i_lvl)
    {
        this->hammer->setUpgradeLvl(i_lvl);
    }
    
    void Player::setSwordLvl(int i_lvl)
    {
        this->sword->setUpgradeLvl(i_lvl);
    }
    
    void Player::setShieldLvl(int i_lvl)
    {
        this->shield->setUpgradeLvl(i_lvl);
    }
    
    void Player::setBowLvl(int i_lvl)
    {
        this->bow->setUpgradeLvl(i_lvl);
    }
