#include "Player.h"
#include "FileManager.h"

Player::Player(const float& maxHealth)
    : Entity(maxHealth)
{
    this->hitbox = new Hitbox(PLAYER, this->shape.getSize().x,this->shape.getSize().y, this->vf_position.x,this->vf_position.y);

    this->i_nchunks = 4;

    this->sword = new Weapon(0.3f, 0.1f, 0.1f, 40, 60, 30, true);
    sword->setUpgradeLvl(0);
    this->hammer = new Weapon(1.0f, 0.7f, 0.2f, 60, 70, 60, true);
    this->shield = new Shield(0.2f, 0.2f, 0.05f, 0.02f);
    this->bow = new RangedWeapon(0.6f, 0.1f, 20, true, this->b_facingRight);

    this->animation = new AnimationComponent(this->shape);
    this->animation->loadAnimationsFromJSON("animations/pengo.json");
}

Player::~Player()
{
    delete this->sword;
    delete this->hammer;
    delete this->bow;
    delete this->shield;
}

void Player::getHurt(float& damage)
{
    damage *= this->shield->DamageBlock();
    this->Entity::getHurt(damage);
    f_regenerationDelta = 0.0f;
}

void Player::getHealed(float& healing)
{
    this->Entity::getHealed(healing);
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

void Player::pickCoin(int value)
{
    this->i_coins += value;
}

void Player::addKill(int amount)
{
    this->i_kills += amount;
}

void Player::unlockWeapon(std::string weaponName)
{
    if(weaponName == "Sword")
    {
        if(atoi(this->getSwordLvl().c_str()) < 0)
            this->setSwordLvl(0);
    }
    else if(weaponName == "Hammer")
    {
        if(atoi(this->getHammrLvl().c_str()) < 0)
            this->setHammrLvl(0);
    }
    else if(weaponName == "Bow")
    {
        if(atoi(this->getBowLvl().c_str()) < 0)
            this->setBowLvl(0);
    }
    else if(weaponName == "Shield")
    {
        if(atoi(this->getShieldLvl().c_str()) < 0)
            this->setShieldLvl(0);
    }
}

bool Player::getIsWeaponUnlocked(std::string weaponName)
{
    bool result = false;
    if(weaponName == "Sword")
    {
        result = (atoi(this->getSwordLvl().c_str()) >= 0);
    }
    else if(weaponName == "Hammer")
    {
        result = (atoi(this->getHammrLvl().c_str()) >= 0);
    }
    else if(weaponName == "Bow")
    {
        result = (atoi(this->getBowLvl().c_str()) >= 0);
    }
    else if(weaponName == "Shield")
    {
        result = (atoi(this->getShieldLvl().c_str()) >= 0);
    }
    return result;
}

void Player::die()
{
    //this->f_score /= 2;
}

void Player::trulyDie()
{
    this->die();
    this->b_isDead = true;
}

void Player::linkWorldProjectiles(std::vector<Projectile*>& proyectileArray)
{
    if(this->bow) this->bow->linkWorldProjectiles(proyectileArray);
}

bool Player::checkObstacle(Hitbox* hitbox)
{
    HitboxType type = hitbox->getType();
    bool result = false;
    switch(type)
    {
        case PLATFORM:
        case ENEMY:
        case BREAKABLE_DOOR:
        result = true; break;
        default: break;
    }

    return result;
}

bool Player::checkInteraction(Hitbox* hitbox)
{
    HitboxType type = hitbox->getType();
    bool result = false;
    switch(type)
    {
        case ENEMY_ATTACK:
        case LETHAL:
        result = true; break;
        default: break;
    }

    return result;
}

void Player::resizeItems(sf::Vector2f scaleRatio)
{
    this->sword->scale(scaleRatio);
    this->hammer->scale(scaleRatio);
    this->shield->scale(scaleRatio);
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
        this->jump(0,1);

    if(!b_mutexAttack && getIsWeaponUnlocked("Sword") && sf::Keyboard::isKeyPressed(sf::Keyboard::G))
    {
        this->sword->startAttack();
        this->b_mutexAttack = true;
    }
    else if(!b_mutexAttack && getIsWeaponUnlocked("Hammer") && sf::Keyboard::isKeyPressed(sf::Keyboard::H))
    {
        this->hammer->startAttack();
        this->b_mutexAttack = true;
    }
    else if(!b_mutexAttack && getIsWeaponUnlocked("Bow") && sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        this->bow->startAttack();
        this->b_mutexAttack = true;
    }
    else if(getIsWeaponUnlocked("Shield") && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        this->shield->RiseShield();
    }
    else
    {
        this->shield->RestShield();
    }   

    // Update the attacks
    bool endSword = this->updateWeapon(sword);
    bool endHammer = this->updateWeapon(hammer);
    bool endBow = this->updateWeapon(bow);
    b_mutexAttack = (endSword || endHammer || endBow);

    // Update shield state
    this->shield->setPosition(this->vf_position.x, this->vf_position.y, this->b_facingRight);

    // Update life regeneration
    this->regenerate();

    // Update general stuff
    this->Entity::update();
}

void Player::render()
{
    Engine::getInstance()->renderDrawable(&shape);
    this->sword->render();
    this->hammer->render();
    this->shield->render();
    //this->hitbox->render();
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

std::string Player::getScore(){

    return std::to_string(this->i_score).c_str();

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
}

std::string Player::getNear()
{
    return nearDialogue;
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

void Player::setNear( std::string text){
    this->nearDialogue = text;
}
