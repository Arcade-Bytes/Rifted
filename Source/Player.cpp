#include "Player.h"
#include "FileManager.h"

Player::Player()
    : Entity()
{
    this->sword = new Weapon(0.3f, 0.1f, 40, 60);
    this->hammer = new Weapon(1.0f, 0.8f, 60, 70);
    this->shield = new Shield(0.2f, 0.2f, 0.05f, 0.02f);
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
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::F6))
    {
        /*
        std::cout << ftl::LoadByKey("name") <<'\n';
        std::cout << ftl::LoadByKey("level") <<'\n';
        std::cout << ftl::LoadByKey("secret") <<'\n';
        */
       std::string bully[] = {"player","upgrades","health"};
       std::cout << ftl::LoadByKey(bully) << '\n';
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
    {
       ftl::ModifyByKey("name","DOOM SLAYER");
       ftl::ModifyByKey("level","YES");
       ftl::ModifyByKey("secret","NO SECRET LMAO");   
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
}
