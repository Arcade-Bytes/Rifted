#include "Player.h"

Player::Player(const float& maxHealth)
    : Entity(maxHealth)
{
    this->sword = new Weapon(0.3f, 0.1f, 0.1f, 40, 60, 30, true);
    this->hammer = new Weapon(1.0f, 0.7f, 0.2f, 60, 70, 60, true);
    this->shield = new Shield(0.2f, 0.2f, 0.05f, 0.02f);

    this->animation = new AnimationComponent(this->shape);
    this->animation->loadAnimationsFromJSON("animations/pengo.json");
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

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::G))
        this->sword->startAttack();
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        this->hammer->startAttack();

    // Update the attacks
    this->updateWeapon(sword);
    this->updateWeapon(hammer);

    // Update shield state
    this->shield->setPosition(this->vf_position.x, this->vf_position.y, this->b_facingRight);
    this->shield->update();

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
    this->hitbox->render();
}
