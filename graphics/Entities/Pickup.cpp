#include "Pickup.hpp"
#include "Spaceship.hpp"
#include "../Utility.hpp"
#include "../DataTable.hpp"
#include "../CollisionResponseMap.hpp"

namespace
{
    const std::vector<PickupData> table = initializePickupData();
}
bool Pickup::mHasInitializedResponses = false;

Pickup::Pickup(Type type, const TextureHolder& textures, ObjectContext context)
    : Entity(1, true, context),
      mType(type),
      mSprite(textures.get(table[type].texture), table[type].textureRect)
{
    addCategories(Category::Pickup);
    centerOrigin(mSprite);

    if(!mHasInitializedResponses)
        initializeCollisionResponses();
}

sf::FloatRect Pickup::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Pickup::apply(Spaceship& player) const
{
    table[mType].action(player);
}

const sf::IntRect& Pickup::getTextureRect() const
{
    return mSprite.getTextureRect();
}

const sf::Texture* Pickup::getTexture() const
{
    return mSprite.getTexture();
}

sf::Transform Pickup::getSpriteInverseTransform() const
{
    return mSprite.getInverseTransform();
}

void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void Pickup::initializeCollisionResponses()
{
    mHasInitializedResponses = true;

    auto response1 = castResponse<Spaceship, Pickup>([](Spaceship& player, Pickup& pickup)
    {
        pickup.apply(player);
        pickup.destroy();
    });

    Category::Type cat1 = static_cast<Category::Type>(Category::PlayerSpaceship | Category::Collidable);
    Category::Type cat2 = static_cast<Category::Type>(Category::Pickup          | Category::Collidable);

    CollisionResponseMap::addResponse(cat1, cat2, response1);
}
