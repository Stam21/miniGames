#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Entity.hpp"
#include "../ResourcesID.hpp"
#include "../AttackSystem/AttackPattern.hpp"
#include <SFML/Graphics/Sprite.hpp>

class Projectile : public Entity
{
    public:
                                        Projectile(Projectiles::ID, const TextureHolder&,
                                                   ObjectContext, int shooterID, float speed, bool isEnemy);
        float                           getMaxSpeed() const;
        float                           getDamage() const;
        sf::FloatRect                   getBoundingRect() const override;
        int                             getShootersID() const;
        void                            setPattern(AttackPattern::ID);
        void                            setPatternData(AttackPattern::PatternData);
        AttackPattern::ID               getPattern() const;
        AttackPattern::PatternData      getPatternData() const;
        void                            setDirection(sf::Vector2f);
        sf::Vector2f                    getDirection() const;
        void                            setStartPos(sf::Vector2f);
        sf::Vector2f                    getStartPos() const;
        float                           activeTime() const;
        void                            updateTime(sf::Time dt);
        const sf::IntRect& 	            getTextureRect() const override;
        const sf::Texture*              getTexture() const override;
        sf::Transform                   getSpriteInverseTransform() const override;
        void                            onRemoval();

    protected:
        Projectiles::ID getType() const;

    private:
        virtual void    updateCurrent(sf::Time, CommandQueue&) override;
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;
        static void     initializeCollisionResponses();

        Projectiles::ID                 mType;
        sf::Sprite                      mSprite;
        int                             mShooterID;
        AttackPattern::ID               mPattern;
        AttackPattern::PatternData      mPatternData;
        float                           mSpeed;
        sf::Vector2f                    mDirection;
        sf::Vector2f                    mStartPos;
        sf::Time                        mTimeActive;
        bool                            mIsEnemy;
        const TextureHolder&            mTextures;
        static bool                     mHasInitializedResponses;
};

#endif // PROJECTILE_HPP
