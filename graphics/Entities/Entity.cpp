#include "Entity.hpp"
#include "../World.hpp"
#include "../AnimationNode.hpp"
int Entity::mNextFreeID = 0;

Entity::Entity(float hitpoints, bool isCollidable, ObjectContext context)
    : mHitpoints(hitpoints),
      mContext(context),
      mIsCollidable(isCollidable),
      mID(mNextFreeID++),
      mPerfectCollisionFlag(true)
{
    if(isCollidable)
        addCategories(Category::Collidable);
}

void Entity::resetIDs()
{
    mNextFreeID = 0;
}

const sf::IntRect& Entity::getTextureRect() const
{
}

const sf::Texture* Entity::getTexture() const
{
}

sf::Transform Entity::getSpriteInverseTransform() const
{
}

float Entity::getHitpoints() const
{
    return mHitpoints;
}

void Entity::setHitpoints(float hitpoints)
{
    mHitpoints = hitpoints;
}

sf::Vector2f Entity::getVelocity() const
{
    return mVelocity;
}

void Entity::setVelocity(sf::Vector2f velocity)
{
    mVelocity = velocity;
}

void Entity::setVelocity(float x, float y)
{
    setVelocity(sf::Vector2f(x, y));
}

void Entity::accelerate(sf::Vector2f velocity)
{
    mVelocity += velocity;
}

void Entity::accelerate(float x, float y)
{
    mVelocity.x += x;
    mVelocity.y += y;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    move(mVelocity * dt.asSeconds());
}

void Entity::damage(float hitpoints)
{
    if(hitpoints > 0)
        mHitpoints -= hitpoints;
}

void Entity::repair(float hitpoints)
{
    if(hitpoints > 0)
        mHitpoints += hitpoints;
}

void Entity::destroy()
{
    mHitpoints = 0;
}

const ObjectContext& Entity::getObjectContext() const
{
    return mContext;
}

sf::FloatRect Entity::getLocalBounds() const
{
    return sf::FloatRect();
}

void Entity::removeEntity()
{
    destroy();
}

bool Entity::isMarkedForRemoval() const
{
    if(mHitpoints <= 0)
        return true;

    return false;
}

bool Entity::isCollidable() const
{
    return mIsCollidable;
}

bool collision(const Entity& lhs, const Entity& rhs)
{
    return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

int Entity::getEntityID() const
{
    return mID;
}

void Entity::enablePerfectCollision(bool flag)
{
    mPerfectCollisionFlag = flag;
}

bool Entity::isPerfectCollisionEnabled() const
{
    return mPerfectCollisionFlag;
}

void Entity::sendExplosion(sf::Vector2f pos, const TextureHolder& t, float scale, float delaySeconds, float volumeMultiplier) const
{
    auto soundPlayer = getObjectContext().soundPlayer;

    Command explosionCommand;
    explosionCommand.mCategories = Category::AirLayer;
    explosionCommand.mAction = [pos, &t, scale, delaySeconds, soundPlayer, volumeMultiplier](SceneNode& layer, sf::Time)
    {
        std::unique_ptr<AnimationNode> node(new AnimationNode(Animation::Explosion, sf::seconds(0.06f), t, pos));
        node->setScale(scale, scale);
        node->setDelay(sf::seconds(delaySeconds));
        layer.attachChild(std::move(node));
        soundPlayer->play(Sound::Explosion, pos, volumeMultiplier, delaySeconds);
    };

    getObjectContext().commandQueue->push(explosionCommand);
}
