#ifndef BAR_HPP
#define BAR_HPP

#include "../SceneNode.hpp"
#include "Entity.hpp"
#include "Spaceship.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <array>

class Bar : public SceneNode
{
    public:
        enum Type
        {
            Health,
            Boost,
            BossHealth
        };

                        Bar(float val, float maxValue, sf::Vector2f maxSize, Type);
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;
        virtual void    updateCurrent(sf::Time, CommandQueue&) override;
        void            setPosition(sf::Vector2f position);
        void            setPosition(float x, float y);
        void            setColorRange(sf::Color, sf::Color);
        void            updateValue(float);

    private:
        sf::RectangleShape  mBar;
        sf::RectangleShape  mBarContainer;
        float               mMaxValue;
        float               mCurrentValue;
        sf::Vector2f        mMaxSize;
        sf::Color           mMaxColor;
        std::array<int, 4>  mColorRange;
        bool                mAbsolutePosition;
};

#endif // BAR_HPP
