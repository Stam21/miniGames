#include "AmmoNode.hpp"
#include "../Utility.hpp"

AmmoNode::AmmoNode(int id, const TextureHolder& textures, const FontHolder& fonts, const sf::View& view)
    : mPlayerID(id),
      mView(view),
      mSprite(textures.get(Textures::Projectiles), Textures::MissileRect),
      mAmmo(0)
{
    mAmount.setFont(fonts.get(Fonts::BPmonoItalics));
    mAmount.setCharacterSize(31u);

    mAmmoGetter.mCategories = Category::PlayerSpaceship;
    mAmmoGetter.mAction = castFunctor<Spaceship>([this](Spaceship& player, sf::Time)
    {
        if(mPlayerID == player.getPlayerID())
            mAmmo = player.getMissileAmmo();
    });
}

void AmmoNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    commands.push(mAmmoGetter);

    if(mAmmo < 999)
        mAmount.setString(toString(mAmmo));

    auto viewSize = mView.getSize();
    auto textSpriteSpace = mAmount.getLocalBounds().width + 8.f;

    if(mAlignment == Align::Left)
    {
        mAmount.setPosition(viewSize.x * mMargin, viewSize.y - 50.f);
        mSprite.setPosition(mAmount.getPosition().x + textSpriteSpace, viewSize.y - 45.f);
    }
    else
    {
        mAmount.setPosition(viewSize.x * (1-mMargin) - textSpriteSpace - mSprite.getLocalBounds().width, viewSize.y - 50.f);
        mSprite.setPosition(mAmount.getPosition().x + textSpriteSpace, viewSize.y - 45.f);
    }

    mAmmo = 0;
}

void AmmoNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
    target.draw(mAmount, states);
}

///@param percent - distance in % from border it's aligned to
void AmmoNode::setMargin(float percent)
{
    mMargin = percent / 100.f;
}

void AmmoNode::align(Align a)
{
    mAlignment = a;
}
