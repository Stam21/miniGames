#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../SceneNode.hpp"
#include "../ObjectContext.hpp"
#include "../ResourcesID.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <list>
class World;

class Entity : public SceneNode
{
    public:
        explicit                    Entity(float hitpoints, bool isCollidable, ObjectContext);
        float                       getHitpoints() const;
        void                        setHitpoints(float hitpoints);
        sf::Vector2f                getVelocity() const;
        void                        setVelocity(sf::Vector2f);
        void                        setVelocity(float x, float y);
        void                        accelerate(sf::Vector2f);
        void                        accelerate(float x, float y);
        void                        damage(float hitpoints);
        void                        repair(float hitpoints);
        void                        destroy();
        const ObjectContext&        getObjectContext() const;
        virtual sf::FloatRect       getLocalBounds() const;
        virtual void                removeEntity();
        virtual bool                isMarkedForRemoval() const override;
        bool                        isCollidable() const;
        int                         getEntityID() const;
        static void                 resetIDs();
        virtual const sf::IntRect& 	getTextureRect() const;
        virtual const sf::Texture* 	getTexture() const;
        virtual sf::Transform       getSpriteInverseTransform() const;
        void                        enablePerfectCollision(bool);
        bool                        isPerfectCollisionEnabled() const;

    protected:
        void    updateCurrent(sf::Time dt, CommandQueue&) override;
        void    sendExplosion(sf::Vector2f pos, const TextureHolder&, float scale = 1, float delaySeconds = 0, float volumeMultiplier = 1) const;


    private:
        sf::Vector2f            mVelocity;
        float                   mHitpoints;
        ObjectContext           mContext;
        bool                    mIsCollidable;
        int                     mID;
        static int              mNextFreeID;
        bool                    mPerfectCollisionFlag;
};

bool collision(const Entity& lhs, const Entity& rhs);

#endif // ENTITY_HPP
