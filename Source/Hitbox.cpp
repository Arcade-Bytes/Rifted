#include "Hitbox.h"

std::vector<Hitbox*> Hitbox::hitboxes;

Hitbox::Hitbox(HitboxType type, const float& xsize, const float& ysize, const float& xpos, const float& ypos,
        float damage, sf::Vector2f knockback, DamageType damageTypeVal)
    :f_damage(damage), vf_knockback(knockback), damageType(damageTypeVal)
{
    this->type = type;
    this->vf_position = {xpos,ypos};
    this->vf_size = {xsize,ysize};

    this->shape.setFillColor(sf::Color(200,200,200,100));
    this->shape.setSize(this->vf_size);
    this->shape.setOrigin(this->vf_size.x/2,this->vf_size.y/2);
    this->shape.setPosition(this->vf_position);

    hitboxes.push_back(this);
}

Hitbox::~Hitbox()
{
    hitboxes.erase(
        std::find(hitboxes.begin(), hitboxes.end(), this)
    );
}

// Static list management
void Hitbox::resetHitboxLists()
{
    hitboxes.clear();
}

std::vector<Hitbox*>* Hitbox::getAllHitboxes()
{
    return &hitboxes;
}

// Game related
void Hitbox::setType(HitboxType newType)
{
    this->type = newType;
}

HitboxType Hitbox::getType()
{
    return this->type;
}

void Hitbox::setDamage(float damage)
{
    this->f_damage = damage;
}

float Hitbox::getDamage()
{
    return this->f_damage;
}

void Hitbox::setKnockback(sf::Vector2f newKnockback)
{
    this->vf_knockback = newKnockback;
}

sf::Vector2f Hitbox::getKnockback()
{
    return this->vf_knockback;
}

DamageType Hitbox::getDamageType()
{
    return this->damageType;
}

// Position related
sf::Vector2f Hitbox::getPosition()
{
    return this->vf_position;
}

void Hitbox::setPosition(const float& x, const float& y)
{
    this->vf_position.x = x;
    this->vf_position.y = y;
    this->shape.setPosition(this->vf_position);
}

sf::Vector2f Hitbox::getSize()
{
    return this->vf_size;
}

void Hitbox::setSize(const float& x, const float& y)
{
    this->vf_size = {x,y};
    this->shape.setSize(this->vf_size);
    this->shape.setOrigin(this->vf_size.x/2,this->vf_size.y/2);
}

void Hitbox::scale(sf::Vector2f scaleRatio)
{
    this->vf_size.x *= scaleRatio.x;
    this->vf_size.y *= scaleRatio.y;
    this->setSize(vf_size.x, vf_size.y);
}


// Collision related
bool Hitbox::checkBooleanCollision(Hitbox* other)
{
    sf::Vector2f col = this->checkCollision(other);
    return (col.x!=0.0f || col.y != 0.0f);
}

sf::Vector2f Hitbox::checkCollision(Hitbox* other)
{
    sf::Vector2f intersection = {0.0f, 0.0f};
    if(this == other) return intersection;
    
    if(this->shape.getGlobalBounds().intersects(other->getBounds()))
    {
        float thisHalfWidth = shape.getGlobalBounds().width / 2.0f;
        float thisHalfHeight = shape.getGlobalBounds().height / 2.0f;
        float otherHalfWidth = other->getBounds().width / 2.0f;
        float otherHalfHeight = other->getBounds().height / 2.0f;

        sf::Vector2f distance = this->getPosition() - other->getPosition();
        intersection = {
            abs(distance.x) - thisHalfWidth - otherHalfWidth,
            abs(distance.y) - thisHalfHeight - otherHalfHeight
        };

        // Corrects intersection sign for push direction
        if(distance.x > 0) intersection.x *= -1;
        if(distance.y > 0) intersection.y *= -1;
    }
    return intersection;
}

sf::FloatRect Hitbox::getBounds()
{
    return this->shape.getGlobalBounds();
}

// Visual related
void Hitbox::setColor(sf::Color color)
{
    this->shape.setFillColor(color);
}

void Hitbox::update()
{
 
}

void Hitbox::render()
{
    Engine::getInstance()->renderDrawable(&shape);
}
