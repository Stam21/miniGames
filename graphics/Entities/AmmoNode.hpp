#ifndef AMMONODE_HPP
#define AMMONODE_HPP

#include "../SceneNode.hpp"
#include "../ResourcesID.hpp"
#include "../CommandQueue.hpp"
#include "../Command.hpp"
#include "Spaceship.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class AmmoNode : public SceneNode
{
    public:
        enum Align{Left, Right};
                        AmmoNode(int playerID, const TextureHolder&, const FontHolder&, const sf::View&);
        void            setMargin(float percent);
        void            align(Align);

    private:
        virtual void    updateCurrent(sf::Time, CommandQueue&) override;
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

        int                 mPlayerID;
        const sf::View&     mView;
        sf::Sprite          mSprite;
        sf::Text            mAmount;
        float               mMargin;
        Align               mAlignment;
        int                 mAmmo;
        Command             mAmmoGetter;
};

#endif // AMMONODE_HPP
