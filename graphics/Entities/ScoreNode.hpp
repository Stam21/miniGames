#ifndef SCORENODE_HPP
#define SCORENODE_HPP

#include "../SceneNode.hpp"
#include "../ResourcesID.hpp"
#include <SFML/Graphics/Text.hpp>
#include <unordered_map>

class ScoreNode : public SceneNode
{
    public:
                                        ScoreNode(const FontHolder& fonts);
        void                            show(bool);
        std::unordered_map<int, int>    getPlayersScoresMap();

    private:
        virtual void updateCurrent(sf::Time, CommandQueue&) override;
        virtual void drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

        std::unordered_map<int, int>    mPlayersScores; // id, score
        sf::Text                        mScore;
        Command                         mScoreUpdater;
        bool                            mIsVisible;
};

#endif // SCORENODE_HPP
