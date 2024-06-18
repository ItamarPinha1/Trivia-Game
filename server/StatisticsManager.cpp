#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(const std::weak_ptr<IDatabase> database) : m_database(database)
{
}


std::vector<HighScores> StatisticsManager::getHighScore() const
{
    if (!m_database.expired())
    {
        return m_database.lock()->getHighScores();
    }
    else
    {
        return {};
    }
}

UserStatistics StatisticsManager::getUserStatistics(const std::string& username) const
{
    if (!m_database.expired())
    {
        UserStatistics userStatistics;
        auto db = m_database.lock();
        userStatistics.avarageAnswerTime = db->getPlayerAverageAnswerTime(username);
        userStatistics.numOfCorrectAnswers = db->getNumOfCorrectAnswers(username);
        userStatistics.numOfTotalAnswers = db->getNumOfTotalAnswers(username);
        userStatistics.numOfPlayerGames = db->getNumOfPlayerGames(username);
        userStatistics.playerScore = db->getPlayerScore(username);

        return userStatistics;
    }
    else
    {
        return {};
    }
}