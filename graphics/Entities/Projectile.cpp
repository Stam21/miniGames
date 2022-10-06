#include "Projectile.hpp"
#include "../DataTable.hpp"
#include "../Utility.hpp"
#include "../World.hpp"
#include "../ProjectileInfo.hpp"
#include "../ParticleSystem/EmitterNode.hpp"
#include "../CollisionResponseMap.hpp"
#include <unordered_map>
#include <memory>
using ProjectileInfo::table;
bool Projectile::mHasInitializedResponses = false;

Projectile::Projectile(Projectiles::ID type, const TextureHolder& textures,
                        ObjectContext context, int shooterID, float speed, bool isEnemy)
    : Entity(table.at(type).hitpoints, true, context),
      mType(type),
      mSprite(textures.get(table.at(type).texture), table.at(type).textureRect),
      mShooterID(shooterID),
      mSpeed(speed),
      mIsEnemy(isEnemy),
      mTextures(textures)
{
    if(isEnemy)
        addCategories(Category::EnemyProjectile);
    else
        addCategories(Category::AlliedProjectile);

    centerOrigin(mSprite);

    if(mType == Projectiles::Missile) // Adds emitters for missiles
    {
        std::unique_ptr<EmitterNode> smoke(new EmitterNode(Particle::Smoke, getObjectContext().particleNode, 30.f));
        smoke->setPosition(0.f, mSprite.getLocalBounds().height / 2.f);
        attachChild(std::move(smoke));

        std::unique_ptr<EmitterNode> propellant(new EmitterNode(Particle::Propellant, getObjectContext().particleNode, 30.f));
        propellant->setPosition(0.f, mSprite.getLocalBounds().height / 2.f);
        attachChild(std::move(propellant));

        getObjectContext().soundPlayer->play(Sound::Missile, 0.6f);
    }

    if(!mHasInitializedResponses)
        initializeCollisionResponses();
}

float Projectile::getMaxSpeed() const
{
    return mSpeed;
}

float Projectile::getDamage() const
{
    return table.at(mType).damage;
}

sf::FloatRect Projectile::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

int Projectile::getShootersID() const
{
    return mShooterID;
}

void Projectile::setPattern(AttackPattern::ID id)
{
    mPattern = id;
}

void Projectile::setPatternData(AttackPattern::PatternData data)
{
    mPatternData = data;
}

AttackPattern::ID Projectile::getPattern() const
{
    return mPattern;
}

AttackPattern::PatternData Projectile::getPatternData() const
{
    return mPatternData;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    float angle = std::atan2(getVelocity().y, getVelocity().x);
    setRotation(toDegree(angle) + 90.f);

    Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void Projectile::initializeCollisionResponses()
{
    mHasInitializedResponses = true;

    auto response1 = castResponse<Spaceship, Projectile>([](Spaceship& player, Projectile& projectile)
    {
        auto dmg = projectile.getDamage();
        player.damage(dmg);
        projectile.damage(dmg);
    });
    auto response2 = castResponse<Spaceship, Projectile>([](Spaceship& enemy, Projectile& projectile)
    {
        auto dmg = projectile.getDamage();
        enemy.damage(dmg);
        projectile.damage(dmg);
        enemy.setAttackerID(projectile.getShootersID());
    });

    Category::Type cat1 = static_cast<Category::Type>(Category::PlayerSpaceship  | Category::Collidable);
    Category::Type cat2 = static_cast<Category::Type>(Category::EnemyProjectile  | Category::Collidable);
    Category::Type cat3 = static_cast<Category::Type>(Category::EnemySpaceship   | Category::Collidable);
    Category::Type cat4 = static_cast<Category::Type>(Category::AlliedProjectile | Category::Collidable);

    CollisionResponseMap::addResponse(cat1, cat2, response1);
    CollisionResponseMap::addResponse(cat3, cat4, response2);
}

void Projectile::setDirection(sf::Vector2f dir)
{
    mDirection = dir;
}

sf::Vector2f Projectile::getDirection() const
{
    return mDirection;
}

void Projectile::setStartPos(sf::Vector2f pos)
{
    mStartPos = pos;
}

sf::Vector2f Projectile::getStartPos() const
{
    return mStartPos;
}

///@return seconds that elapsed since creation of this projectile
float Projectile::activeTime() const
{
    return mTimeActive.asSeconds();
}

void Projectile::updateTime(sf::Time dt)
{
    mTimeActive += dt;
}

const sf::IntRect& Projectile::getTextureRect() const
{
    return mSprite.getTextureRect();
}

const sf::Texture* Projectile::getTexture() const
{
    return mSprite.getTexture();
}

sf::Transform Projectile::getSpriteInverseTransform() const
{
    return mSprite.getInverseTransform();
}

Projectiles::ID Projectile::getType() const
{
    return mType;
}

void Projectile::onRemoval()
{
    if(mType == Projectiles::Missile)
        sendExplosion(getWorldPosition(), mTextures, 0.5f, 0.f, 0.3f);
}
