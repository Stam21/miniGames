#ifndef PICKUP_HPP
#define PICKUP_HPP

#include "Entity.hpp"
#include "../ResourcesID.hpp"
#include "../Category.hpp"
#include "../Command.hpp"
#include <SFML/Graphics/Sprite.hpp>
class Spaceship;

class Pickup : public Entity
{
    public:
        enum Type
        {
            FireRate,
            FireSpread,
            MissileRefill,
            HealthRefill,
            TypeCount
        };

                            Pickup(Type, const TextureHolder&, ObjectContext);
        sf::FloatRect       getBoundingRect() const override;
        void                apply(Spaceship&) const;
        const sf::IntRect& 	getTextureRect() const override;
        const sf::Texture*  getTexture() const override;
        sf::Transform       getSpriteInverseTransform() const override;

    private:
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;
        static void     initializeCollisionResponses();

        Type             mType;
        sf::Sprite       mSprite;
        static bool      mHasInitializedResponses;
};

#endif // PICKUP_HPP
