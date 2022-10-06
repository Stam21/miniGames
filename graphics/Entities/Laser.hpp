#ifndef LASER_HPP
#define LASER_HPP

#include "Projectile.hpp"
#include <vector>

class Laser : public Projectile
{
    public:
                                Laser(Projectiles::ID, const TextureHolder&, ObjectContext,
                                       int shooterID, float speed, bool isEnemy);
        virtual sf::FloatRect   getBoundingRect() const override;

    private:
        virtual void            updateCurrent(sf::Time, CommandQueue&) override;
        virtual void            drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;
        void                    updateVertexArray() const;
        bool                    createLaserPart(sf::IntRect textureRect, float laserPartHeight, sf::Vector2f& offset) const;

        mutable sf::VertexArray     mLaser;
        const TextureHolder&        mTextures;
        bool                        mNeedsVertexUpdate;
        mutable bool                mIsMaxLength;
};

#endif // LASER_HPP
