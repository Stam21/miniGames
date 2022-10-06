#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include "Projectile.hpp"
#include "../Category.hpp"
#include "../Command.hpp"
#include "../CommandQueue.hpp"
#include "../ResourcesID.hpp"
#include "../SpriteNode.hpp"
#include "../AttackSystem/AttackManager.hpp"
#include <SFML/System/Time.hpp>
#include <memory>
class Entity;
class Bar;

class Spaceship : public Entity
{
    public:
        enum class EnemyType{Normal, Boss};
        using SpriteNodePtr = std::unique_ptr<SpriteNode>;

                                Spaceship(int, const TextureHolder&, sf::Vector2f pos, ObjectContext,
                                          const std::vector<Spaceship*>& targets, int id = -1);
        void                    increaseFireRate();
        void                    increaseSpread();
        int                     getMissileAmmo() const;
        void                    setMissileAmmo(int);
        void                    changeMissileAmmo(int);
        int                     getPlayerID() const;
        void                    setIdentifier(int);
        void                    fire();
        void                    boostSpeed();
        void                    trySpeedBoost();
        void                    launchMissile();
        float                   getMaxSpeed() const;
        virtual sf::FloatRect   getLocalBounds() const override;
        virtual sf::FloatRect   getBoundingRect() const override;
        virtual void            removeEntity() override;
        void                    increaseScore(int);
        int                     getScore() const;
        void                    setAttackerID(int);
        const sf::IntRect& 	    getTextureRect() const override;
        const sf::Texture* 	    getTexture() const override;
        sf::Transform           getSpriteInverseTransform() const override;

    protected:
        virtual void            updateCurrent(sf::Time, CommandQueue&) override;
        virtual void            drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

    private:
        void            updateRollAnimation(sf::Time);
        void            updateMovementPatterns(sf::Time);
        void            createPickup() const;
        void            createExplosion() const;
        void            changeScore();
        void            increaseScoreRequest(int) const;
        virtual void    onRemoval() override;
        void            updateBoostFuel();
        static void     initializeCollisionResponses();
        void            createBossHealthbar();
        void            hideScore();

        int                     mTypeID;
        int                     mFireRateLevel;
        int                     mSpreadLevel;
        int                     mMissileAmmo;
        int                     mPlayerID;
        bool                    mIsEnemy;
        bool                    mWasKilled;
        float                   mTravelledDistance;
        int                     mDirectionIndex;
        sf::Time                mLastRoll;
        const TextureHolder&    mTextures;
        int                     mAttackerID;
        int                     mScore;
        AttackManager           mAttackManager;
        bool                    mBoosted;
        bool                    mBoostCooldown;
        float                   mBoostFuel;
        Bar*                    mHealthBar;
        Bar*                    mBoostFuelBar;
        SpriteNodePtr           mSprite;
        static bool             mHasInitializedResponses;
};

#endif // SPACESHIP_HPP
