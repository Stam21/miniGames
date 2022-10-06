#include "ScoreNode.hpp"
#include "Spaceship.hpp"
#include "../Utility.hpp"
#include "../ApplicationData.hpp"

ScoreNode::ScoreNode(const FontHolder& fonts)
    : mScore("0", fonts.get(Fonts::BPmonoItalics), 32u),
      mIsVisible(true)
{
    addCategories(Category::ScoreNode);

    mScoreUpdater.mCategories = Category::PlayerSpaceship;
    mScoreUpdater.mAction = castFunctor<Spaceship>([this](Spaceship& player, sf::Time)
    {
        mPlayersScores[player.getPlayerID()] = player.getScore();
    });

    centerOrigin(mScore);
    mScore.setPosition(ApplicationData::WINDOW_WIDTH * 0.5f, 18.f);
}

void ScoreNode::show(bool flag)
{
    mIsVisible = flag;
}

std::unordered_map<int, int> ScoreNode::getPlayersScoresMap()
{
    return mPlayersScores;
}

void ScoreNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    int cumulativeScore = 0;

    for(const auto& playerScore : mPlayersScores)
        cumulativeScore += playerScore.second;

    mScore.setString(toString(cumulativeScore));
    centerOrigin(mScore);
    commands.push(mScoreUpdater);
}

void ScoreNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(mIsVisible)
    {
        auto view = target.getView();
        target.setView(target.getDefaultView());
        target.draw(mScore, states);
        target.setView(view);
    }
}
