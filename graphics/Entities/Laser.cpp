#include "Laser.hpp"
#include "../DataTable.hpp"
namespace
{
    const std::unordered_map<int, LaserData> data = initializeLaserData();
}

Laser::Laser(Projectiles::ID type, const TextureHolder& textures, ObjectContext context,
              int shooterID, float speed, bool isEnemy)
    : Projectile(type, textures, context, shooterID, speed, isEnemy),
      mLaser(sf::Triangles),
      mTextures(textures),
      mNeedsVertexUpdate(true),
      mIsMaxLength(false)
{
    Entity::enablePerfectCollision(false);
    setOrigin(sf::Vector2f(data.at(type).textureRects[0].width * 0.5f, 0.f));

    // Prevents laser from being removed due to not having bounds
    mLaser.append(sf::Vector2f(0.f, 0.f));
    mLaser.append(sf::Vector2f(0.f, 1.f));
    mLaser.append(sf::Vector2f(1.f, 0.f));
}

sf::FloatRect Laser::getBoundingRect() const
{
    return getWorldTransform().transformRect(mLaser.getBounds());
}

void Laser::updateCurrent(sf::Time dt, CommandQueue& commandQueue)
{
    mNeedsVertexUpdate = true;
    Entity::updateCurrent(dt, commandQueue);
}

void Laser::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(mNeedsVertexUpdate && !mIsMaxLength)
        updateVertexArray();

    states.texture = &mTextures.get(data.at(getType()).texture);
    target.draw(mLaser, states);
}

void Laser::updateVertexArray() const
{
    float maxLength = data.at(getType()).maxLength;
    float currentDistance = std::abs(getPosition().y - getStartPos().y);
    float currentLength = std::min(maxLength, currentDistance);
    std::array<float, 3> laserPartsHeights;
    sf::Vector2f offset(0.f, 0.f);

    // If laser reached max length then make it last vertex array update
    if(currentDistance > maxLength)
        mIsMaxLength = true;

    // Fill array with laser parts heights
    const auto& textureRects = data.at(getType()).textureRects;
    float maxBodyPartLength = maxLength - textureRects[0].height - textureRects[2].height;

    laserPartsHeights[0] = std::min(currentLength, static_cast<float>(textureRects[0].height)); // Head
    currentLength -= laserPartsHeights[0];
    laserPartsHeights[1] = std::min(currentLength, static_cast<float>(maxBodyPartLength)); // Body
    currentLength -= laserPartsHeights[1];
    laserPartsHeights[2] = std::min(currentLength, static_cast<float>(textureRects[2].height)); // Tail

    // Clear vertices and add new
    mLaser.clear();
    for(int i=0; i < 3; ++i)
    {
        if(!createLaserPart(textureRects[i], laserPartsHeights[i], offset))
            break;
    }
}

///@return True if part was created, otherwise false
bool Laser::createLaserPart(sf::IntRect textureRect, float laserPartHeight, sf::Vector2f& offset) const
{
    if(laserPartHeight <= 0)
        return false;

    sf::Vertex bottomLeft(sf::Vector2f(offset.x, -offset.y), sf::Vector2f(textureRect.left, textureRect.top + textureRect.height));
    sf::Vertex bottomRight(sf::Vector2f(textureRect.width + offset.x, -offset.y), sf::Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height));
    sf::Vertex topLeft(sf::Vector2f(offset.x, -(laserPartHeight + offset.y)), sf::Vector2f(textureRect.left, textureRect.top));
    sf::Vertex topRight(sf::Vector2f(textureRect.width + offset.x, -(laserPartHeight + offset.y)), sf::Vector2f(textureRect.left + textureRect.width, textureRect.top));

    mLaser.append(bottomLeft);
    mLaser.append(bottomRight);
    mLaser.append(topLeft);

    mLaser.append(topLeft);
    mLaser.append(bottomRight);
    mLaser.append(topRight);

    offset.y += laserPartHeight;
    return true;
}
