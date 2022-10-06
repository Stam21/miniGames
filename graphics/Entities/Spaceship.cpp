#include "Spaceship.hpp"
#include "Pickup.hpp"
#include "Bar.hpp"
#include "../Utility.hpp"
#include "../SpriteNode.hpp"
#include "../World.hpp"
#include "../DataTable.hpp"
#include "../AnimationNode.hpp"
#include "../CollisionResponseMap.hpp"
#include <vector>
#include <memory>
namespace
{
    const std::vector<SpaceshipData> spaceshipInfo = initializeSpaceshipData();
    const std::vector<SpaceshipTextureData> textureInfo = initializeSpaceshipTextureData();
    constexpr float FUEL_MAX = 100.f;
    constexpr float FUEL_MIN = 25.f;
    constexpr float FUEL_BURN_RATE = 4.4f;
    constexpr float FUEL_REFILL_RATE = 0.3f;
}
bool Spaceship::mHasInitializedResponses = false;

Spaceship::Spaceship(int typeID, const TextureHolder& textures, sf::Vector2f pos,
                    ObjectContext context, const std::vector<Spaceship*>& targets, int id)
    : Entity(spaceshipInfo[typeID].hitpoints, true, context),
      mTypeID(typeID),
      mFireRateLevel(6),
      mSpreadLevel(0),
      mMissileAmmo(2),
      mPlayerID(id),
      mIsEnemy(typeID != 0),
      mWasKilled(true),
      mTravelledDistance(0.f),
      mDirectionIndex(0),
      mTextures(textures),
      mAttackerID(-1),
      mScore(0),
      mAttackManager(textures, context, id, !mIsEnemy, &targets),
      mBoosted(false),
      mBoostCooldown(false),
      mBoostFuel(FUEL_MAX),
      mBoostFuelBar(nullptr)
{
    setPosition(pos);

    if(mIsEnemy)
        addCategories(Category::EnemySpaceship);
    else
        addCategories(Category::PlayerSpaceship);

    // Initialize sprite
    int textureID = spaceshipInfo[typeID].textureID;
    int animationID = spaceshipInfo[typeID].animationID;
    if(textureID >= 0)
    {
        mSprite.reset(new SpriteNode(textures.get(textureInfo[textureID].texture), textureInfo[textureID].textureRect));
    }
    else
    {
        auto id = static_cast<Animation::ID>(animationID);
        std::unique_ptr<AnimationNode> sprite(new AnimationNode(id, sf::seconds(0.25f), mTextures));
        sprite->setAnimationType(AnimationNode::AnimationType::ForwardAndBackward);
        sprite->setRepeating(true);
        mSprite = std::move(sprite);
    }

    centerOrigin(*mSprite);
    if(mIsEnemy)
        mSprite->rotate(180.f);

    // Create HealthBar for spaceship
    float offset = (mIsEnemy) ? -0.7f : 0.7f;
    sf::Vector2f barSize(getLocalBounds().width * 0.7f, 4.f);

    Bar::Type barType = (spaceshipInfo[mTypeID].tagID == SpaceshipData::Boss) ? Bar::BossHealth : Bar::Health;
    std::unique_ptr<Bar> healthBar(new Bar(getHitpoints(), spaceshipInfo[mTypeID].hitpoints, barSize, barType));

    if(!spaceshipInfo[mTypeID].tagID == SpaceshipData::Boss)
        healthBar->setPosition(0.f, getLocalBounds().height * offset);
    else
        hideScore();

    mHealthBar = healthBar.get();
    attachChild(std::move(healthBar));

    // Create boost fuel bar for players
    if(!mIsEnemy)
    {
        std::unique_ptr<Bar> fuelBar(new Bar(mBoostFuel, FUEL_MAX, sf::Vector2f(barSize.x, barSize.y - 1.f), Bar::Boost));
        fuelBar->setPosition(0.f, getLocalBounds().height * offset + barSize.y);
        mBoostFuelBar = fuelBar.get();
        attachChild(std::move(fuelBar));
    }

    // Add attacks to manager
    const auto& attacks = spaceshipInfo[typeID].attacks;
    for(const auto& attackPair : attacks)
        mAttackManager.pushAttack(attackPair.first, attackPair.second);

    if(!mHasInitializedResponses)
        initializeCollisionResponses();

    // Delay first boss attack
    if(spaceshipInfo[mTypeID].tagID == SpaceshipData::Boss)
        mAttackManager.forceCooldown(sf::seconds(1.2f));
}

void Spaceship::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    mSprite->update(dt, commands);
    updateMovementPatterns(dt);
    updateRollAnimation(dt);
    Entity::updateCurrent(dt, commands);

    mAttackManager.updatePosition(getWorldPosition());
    mAttackManager.update(dt, commands);

    if(mBoostFuelBar)
    {
        updateBoostFuel();
        mBoostFuelBar->updateValue(mBoostFuel);
    }
    mHealthBar->updateValue(getHitpoints());
}

void Spaceship::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(*mSprite, states);
}

void Spaceship::increaseFireRate()
{
    if(mFireRateLevel < 9)
        ++mFireRateLevel;
}

void Spaceship::increaseSpread()
{
    if(mSpreadLevel < 2)
        ++mSpreadLevel;
}

int Spaceship::getMissileAmmo() const
{
    return mMissileAmmo;
}

void Spaceship::setMissileAmmo(int missiles)
{
    mMissileAmmo = missiles;
}

void Spaceship::changeMissileAmmo(int amount)
{
    mMissileAmmo += amount;
}

int Spaceship::getPlayerID() const
{
    return mPlayerID;
}

void Spaceship::setIdentifier(int id)
{
    mPlayerID = id;
}

void Spaceship::fire()
{
    if(mAttackManager.tryAttack(mSpreadLevel+200, *(getObjectContext().commandQueue)))
    {
        mAttackManager.forceCooldown(mAttackManager.getCooldown() / static_cast<float>(mFireRateLevel));
        getObjectContext().soundPlayer->play(Sound::AllyGun, 0.7f);
    }
}

void Spaceship::boostSpeed()
{
    if(!mBoostCooldown && mBoostFuel >= FUEL_BURN_RATE)
        mBoosted = true;
}

void Spaceship::trySpeedBoost()
{
    if(mBoosted)
        setVelocity(getVelocity() * 2.f);
}

void Spaceship::launchMissile()
{
    if(mMissileAmmo > 0)
    {
        mAttackManager.forceAttack(199, *(getObjectContext().commandQueue), false);
        --mMissileAmmo;
    }
}

float Spaceship::getMaxSpeed() const
{
    return spaceshipInfo[mTypeID].speed;
}

sf::FloatRect Spaceship::getLocalBounds() const
{
    return mSprite->getBoundingRect();
}

sf::FloatRect Spaceship::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite->getBoundingRect());
}

sf::Transform Spaceship::getSpriteInverseTransform() const
{
    return mSprite->getInverseTransform();
}

void Spaceship::updateRollAnimation(sf::Time dt)
{
    int textureID = spaceshipInfo[mTypeID].textureID;
    mLastRoll += dt;

    if(!mIsEnemy && mLastRoll.asSeconds() > 0.15f && textureID >= 0)
    {
        sf::IntRect currentRect = mSprite->getTextureRect();
        sf::IntRect defaultRect = textureInfo[textureID].textureRect;

        if(getVelocity().x > 0.f && currentRect.left < defaultRect.width * (textureInfo[textureID].spriteNumber-1))
        {
            currentRect.left += currentRect.width;
            mLastRoll = sf::Time::Zero;
        }
        else if(getVelocity().x < 0.f && currentRect.left > 0.f)
        {
            currentRect.left -= currentRect.width;
            mLastRoll = sf::Time::Zero;
        }
        else if(getVelocity().x == 0.f && currentRect != defaultRect)
        {
            (currentRect.left > defaultRect.left) ? currentRect.left -= currentRect.width
                                                       : currentRect.left += currentRect.width;
            mLastRoll = sf::Time::Zero;
        }

        mSprite->setTextureRect(currentRect);
    }
}

void Spaceship::updateMovementPatterns(sf::Time dt)
{
    const std::vector<SpaceshipData::Direction>& directions = spaceshipInfo[mTypeID].directions;
    if(!directions.empty())
    {
        if(mTravelledDistance > directions[mDirectionIndex].distance)
        {
            mTravelledDistance = 0.f;
            mDirectionIndex = (mDirectionIndex+1) % directions.size();
        }

        float radians = toRadian(directions[mDirectionIndex].angle);
        float speedPercentage = directions[mDirectionIndex].speedPercentage/100.f;
        float velx = getMaxSpeed() * std::sin(radians) * speedPercentage;
        float vely = getMaxSpeed() * std::cos(radians) * speedPercentage;

        setVelocity(velx, vely);
        mTravelledDistance += getMaxSpeed() * dt.asSeconds() * speedPercentage;
    }
}

void Spaceship::initializeCollisionResponses()
{
    mHasInitializedResponses = true;

    auto response1 = castResponse<Spaceship, Spaceship>([](Spaceship& player, Spaceship& enemy)
    {
        int entityHitpoints = player.getHitpoints();
        player.damage(enemy.getHitpoints());
        enemy.damage(entityHitpoints);
        enemy.setAttackerID(player.getEntityID()); // Sets id of spaceship that will have score increased
    });

    Category::Type cat1 = static_cast<Category::Type>(Category::PlayerSpaceship | Category::Collidable);
    Category::Type cat2 = static_cast<Category::Type>(Category::EnemySpaceship  | Category::Collidable);

    CollisionResponseMap::addResponse(cat1, cat2, response1);
}

void Spaceship::removeEntity()
{
    Entity::removeEntity();
    mWasKilled = false;
}

void Spaceship::increaseScore(int value)
{
    mScore += value;
}

int Spaceship::getScore() const
{
    return mScore;
}

void Spaceship::setAttackerID(int id)
{
    mAttackerID = id;
}

const sf::IntRect& Spaceship::getTextureRect() const
{
    return mSprite->getTextureRect();
}

const sf::Texture* Spaceship::getTexture() const
{
    return mSprite->getTexture();
}

void Spaceship::createPickup() const
{
    if(spaceshipInfo[mTypeID].tagID == SpaceshipData::Boss)
        return;

    if(mIsEnemy && isInRange(1, spaceshipInfo[mTypeID].pickupChance, randomInt(1, 100)))
    {
        const TextureHolder& t = mTextures;
        Command createPickupCommand;

        createPickupCommand.mCategories = Category::AirLayer;
        createPickupCommand.mAction = [pos = getWorldPosition(), context = getObjectContext(), &t](SceneNode& layer, sf::Time)
        {
            auto type = static_cast<Pickup::Type>(randomInt(0, Pickup::TypeCount-1));
            std::unique_ptr<Pickup> pickup(new Pickup(type, t, context));
            pickup->setPosition(pos);
            layer.attachChild(std::move(pickup));
        };

        getObjectContext().commandQueue->push(createPickupCommand);
    }
}

void Spaceship::createExplosion() const
{
    if(spaceshipInfo[mTypeID].tagID == SpaceshipData::Boss)
    {
        sf::Vector2f pos = getWorldPosition();
        const TextureHolder& textures = mTextures;

        Command deathAnimation;
        deathAnimation.mCategories = Category::AirLayer;
        deathAnimation.mAction = [pos, &textures](SceneNode& layer, sf::Time)
        {
            std::unique_ptr<AnimationNode> node(new AnimationNode(Animation::BossDeathAnimation, sf::seconds(0.2f), textures, pos));
            node->setAnimationType(AnimationNode::AnimationType::ForwardAndBackward);
            node->setRepeating(true);
            node->setLifespan(sf::seconds(5.5f));
            layer.attachChild(std::move(node));
        };
        getObjectContext().commandQueue->push(deathAnimation);

        sendExplosion(sf::Vector2f(pos.x - 52.f, pos.y + 8.f),   mTextures, 1.f,   0.f,   0.85f);
        sendExplosion(sf::Vector2f(pos.x + 55.f, pos.y + 8.f),   mTextures, 1.f,   0.f,   0.85f);
        sendExplosion(sf::Vector2f(pos.x - 52.f, pos.y + 34.f),  mTextures, 0.45f, 0.45f, 0.6f);
        sendExplosion(sf::Vector2f(pos.x + 55.f, pos.y + 34.f),  mTextures, 0.45f, 0.45f, 0.6f);
        sendExplosion(sf::Vector2f(pos.x - 52.f, pos.y + 65.f),  mTextures, 0.45f, 0.6f,  0.6f);
        sendExplosion(sf::Vector2f(pos.x + 55.f, pos.y + 65.f),  mTextures, 0.45f, 0.6f,  0.6f);
        sendExplosion(sf::Vector2f(pos.x - 52.f, pos.y + 116.f), mTextures, 0.75f, 0.7f,  0.7f);
        sendExplosion(sf::Vector2f(pos.x + 55.f, pos.y + 116.f), mTextures, 0.75f, 0.7f,  0.7f);

        sendExplosion(sf::Vector2f(pos.x + 100.f,  pos.y - 30.f), mTextures, 1.1f, 0.6f, 0.85f);
        sendExplosion(sf::Vector2f(pos.x - 90.f,  pos.y + 25.f),  mTextures, 1.1f, 1.3f, 0.85f);
        sendExplosion(sf::Vector2f(pos.x,  pos.y + 90.f),         mTextures, 1.f,  2.f,  0.85f);
        sendExplosion(sf::Vector2f(pos.x + 105.f, pos.y + 55.f),  mTextures, 1.f,  2.8f, 0.85f);
        sendExplosion(sf::Vector2f(pos.x - 90.f,  pos.y + 45.f),  mTextures, 1.2f, 3.4f, 0.9f);
        sendExplosion(sf::Vector2f(pos.x + 88.f,  pos.y + 17.f),  mTextures, 1.2f, 4.3f, 0.9f);
        sendExplosion(sf::Vector2f(pos.x, pos.y + 15.f),          mTextures, 2.f,  5.4f, 1.f);
    }
    else
    {
        sf::FloatRect bounds = mSprite->getLocalBounds();
        float area = bounds.width * bounds.height;
        float scale = 1.f;
        float volumeMultiplier = 1.f;

        if(area > 10000.f)
        {
            scale = std::min(area / 10000.f, 2.f);
        }
        else if(area < 10000.f)
        {
            scale = 1 - ((1 - area / 10000.f) * 0.3f);
            volumeMultiplier = scale;
        }

        sendExplosion(getWorldPosition(), mTextures, scale, 0.f, volumeMultiplier);
    }
}

void Spaceship::changeScore()
{
    increaseScoreRequest(spaceshipInfo[mTypeID].score);
}

void Spaceship::increaseScoreRequest(int value) const
{
    Command increaseScoreCommand;
    increaseScoreCommand.mCategories = Category::PlayerSpaceship;
    increaseScoreCommand.mAction = castFunctor<Spaceship>([attacker = mAttackerID, value](Spaceship& spaceship, sf::Time dt)
    {
        if(spaceship.getPlayerID() == attacker)
            spaceship.increaseScore(value);
    });

    getObjectContext().commandQueue->push(increaseScoreCommand);
}

void Spaceship::onRemoval()
{
    if(mWasKilled)
    {
        createPickup();
        createExplosion();
        changeScore();
    }

    mAttackManager.onRemoval();

    if(!spaceshipInfo[mTypeID].eventSchemes.empty())
    {
        for(const auto& eventScheme : spaceshipInfo[mTypeID].eventSchemes)
        {
            LocationEvent event(getWorldPosition(), static_cast<LocationEvent::Event>(eventScheme.eventID));

            for(const auto& parameter : eventScheme.parameters)
                event.addParameter(parameter.first, parameter.second);

            getObjectContext().events->push(std::move(event));
        }
    }
}

void Spaceship::updateBoostFuel()
{
    if(mBoosted)
    {
        mBoostFuel = std::max(mBoostFuel - FUEL_BURN_RATE, 0.f);
        mBoosted = false;
    }
    else
    {
        mBoostFuel = std::min(mBoostFuel + FUEL_REFILL_RATE, FUEL_MAX);

        if(mBoostFuel < FUEL_MIN)
            mBoostCooldown = true;
        else
            mBoostCooldown = false;
    }
}

void Spaceship::hideScore()
{
    LocationEvent hideScoreEvent(getWorldPosition(), LocationEvent::ShowScore);
    hideScoreEvent.addParameter("showScore", "false");

    getObjectContext().events->push(hideScoreEvent);
}
