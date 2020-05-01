#include "Player.h"
#include "FileManager.h"

//Point variables 

Player::Player(const float& maxHealth)
    : Entity(maxHealth)
{
    this->hitbox = new Hitbox(PLAYER, this->shape.getSize().x,this->shape.getSize().y, this->vf_position.x,this->vf_position.y);

    this->i_nchunks = 4;
    this->i_maxPotions = 2;

    this->f_regenerationDelta = 0.0f;
    this->f_regenerationCooldown = 2.0f;
    this->f_regenerationSpeed = 0.3f;
    this->f_regenerationAmount = 0.5f;

    // Progress key init
    this->vb_mainKeys[0] = this->vb_mainKeys[1] = this->vb_mainKeys[2] = false;

    this->sword = new Weapon(0.3f, 0.1f, 0.1f, 55, 60, 30, true, {500.0f, 400.0f}, LIGHT_ATTACK);
    this->hammer = new Weapon(1.5f, 0.7f, 0.2f, 120, 65, 60, true, {1000.0f, 500.0f}, HEAVY_ATTACK);
    this->shield = new Shield(0.2f, 0.2f, 0.05f, 0.02f);
    this->bow = new RangedWeapon(0.6f, 0.1f, 20, true, this->b_facingRight);

    this->movement->setMaxSpeed(sf::Vector2f(600.0f, 800.0f));
    this->setResistances(0.0f, 0.0f, 0.0f);

    this->animation = new AnimationComponent(this->shape);
    this->animation->loadAnimationsFromJSON("animations/player.json");
}

Player::~Player()
{
    delete this->sword;
    delete this->hammer;
    delete this->bow;
    delete this->shield;
}

float Player::getHurt(float& damage)
{
    damage *= this->shield->DamageBlock();
    float hurtAmount = this->Entity::getHurt(damage);
    f_regenerationDelta = 0.0f;

    if(damage < f_maxHealth)//If it does not get hurt by spikes or instakills (theoretycaly only spikes)
        this->substractPoints(damage);

    return hurtAmount;
}

float Player::getHealed(float& healing)
{
    float healAmount = this->Entity::getHealed(healing);
    f_regenerationDelta = 0.0f;
    return healAmount;
}

void Player::regenerate()
{
    float delta = Engine::getInstance()->getDelta();

    // Get current life chunk
    float chunkSize = f_maxHealth / i_nchunks;
    int i_currentChunk = ceil(f_currentHealth / chunkSize);

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

int Player::getMaxPotions()
{
    return this->i_maxPotions;
}

int Player::getRemainingPotions()
{
    return this->i_potions;
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
    this->f_currentHealth = 1;
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

void Player::updateMovement()
{
    if(this->b_mutexAttack && this->movement->isYStopped()) this->movement->stopX(); // NEW, stop x speed if attacking and in ground only

    Entity::updateMovement();
}

void Player::updateAnimation()
{
    if(this->animation)
    {
        this->s_currentAnimation = "idle";
        
        if(this->b_mutexAttack)
        {
            if(this->sword && this->sword->isAttacking()) this->s_currentAnimation = "attacking_sword";
            if(this->bow && this->bow->isAttacking()) this->s_currentAnimation = "attacking_bow";
            if(this->hammer && this->hammer->isAttacking()) this->s_currentAnimation = "attacking_hammer";
        }
        else if(this->movement->getSpeed().y < 0)
        {
            this->s_currentAnimation = "jumping";
        }
        else if(this->movement->getSpeed().y > 280) // NEW, hardcoded little gap so it doesn't get weird when hitting the ground
        {
            this->s_currentAnimation = "falling";
        }
        else if(abs(this->movement->getSpeed().x) > 0)
        {
            this->s_currentAnimation = "walking";
        }

        this->animation->playAnimation(this->s_currentAnimation, !b_facingRight); // NEW, if I am facing right, don't mirror; else mirror
    }
}

void Player::update()
{
    // Update input
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        this->move(1);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        this->move(-1);
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
    sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
        this->jump(0,1);

    if(!b_mutexAttack && getIsWeaponUnlocked("Sword") && sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    {
        this->sword->startAttack();
        this->b_mutexAttack = true;
    }
    else if(!b_mutexAttack && getIsWeaponUnlocked("Hammer") && sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        this->hammer->startAttack();
        this->b_mutexAttack = true;
    }
    else if(!b_mutexAttack && getIsWeaponUnlocked("Bow") && sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        this->bow->startAttack();
        this->b_mutexAttack = true;
    }
    else if(!b_mutexAttack && this->i_potions > 0 && Engine::getInstance()->getKeyPressed(sf::Keyboard::Num4))
    {
        this->i_potions--;
        this->getHealed(this->f_maxHealth);
    }
    else if(!b_mutexAttack && getIsWeaponUnlocked("Shield") && (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q)))
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

    // Update life regeneration
    this->regenerate();

    // Update general stuff
    this->Entity::update();
}

void Player::render(float frameProgress)
{
    // Interpolate player position
    sf::Vector2f pos = this->getInterpolatedPosition(frameProgress);
    this->shape.setPosition(pos);

    // Interpolate weapons and tools position
    this->sword->setPosition(pos.x, pos.y, this->b_facingRight);
    this->hammer->setPosition(pos.x, pos.y, this->b_facingRight);
    this->shield->setPosition(pos.x, pos.y, this->b_facingRight);

    // Render
    Engine::getInstance()->renderDrawable(&shape);
    this->sword->render();
    this->hammer->render();
    this->shield->render();
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

std::string Player::getPotionsLeft()
{
    return std::to_string(this->i_potions).c_str();
}

std::string Player::getScore()
{
    return std::to_string(this->i_score).c_str();
}

std::string Player::getKeyUnlocked(int index)
{
    if(index >= 0 && index < 3)
    {
        int unlocked = this->vb_mainKeys[index] ? 1 : 0;
        return std::to_string(unlocked).c_str();
    }
    else
    {
        return "0";
    }
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

std::vector<std::string> Player::getNear()
{
    return nearDialogue;
}

void Player::addPoints(int add){

    this->i_score += add;

}

void Player::substractPoints(int substract){

    if(substract < i_score)
        this->i_score -= substract;
    else this->i_score = 0;

}

int Player::getPoints(){

    return this->i_score;

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

    // Recalculate max life and chunks
    float chunkSize = this->f_maxHealth / this->i_nchunks;
    this->i_nchunks = 4 + this->i_healthUpg;
    this->setMaxHealth(chunkSize * this->i_nchunks);
    this->f_currentHealth = this->f_maxHealth;
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

void Player::setRemainingPotions(int i_pot)
{
    this->i_potions = i_pot;
}

void Player::setScore(int i_scr)
{
    this->i_score = i_scr;
}

void Player::setKeyUnlocked(bool unlocked, int keyIndex)
{
    if(keyIndex >= 0 && keyIndex < 3)
    {
        this->vb_mainKeys[keyIndex] = unlocked;
    }
}

void Player::setNear(std::vector<std::string> text)
{
    this->nearDialogue = text;
}
