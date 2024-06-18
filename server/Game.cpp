#include "Game.h"


Game::Game(const std::vector<Question>& questions, const std::map<LoggedUser, GameData>& players, const int id, const int timeForQuestin, const int numOfQuestions) : m_questions(questions), m_players(players), m_gameId(id), m_timeForQuestion(timeForQuestin), m_numOfQuestions(numOfQuestions)
{
    for (auto& player : players)
    {
        m_players[player.first].currentQuestion = questions[0];
        m_players[player.first].correctAnswerCount = 0;
        m_players[player.first].wrongAnswerCount = 0;
        m_players[player.first].avarageAnswerTime = 0;
        m_players[player.first].currentQuestionId = 0;
        m_players[player.first].startTime = std::chrono::high_resolution_clock::now();
    }
}


Question Game::getQuestionForUser(const LoggedUser& user)
{
    auto startTime = m_players[user].startTime;
    auto currentTime = std::chrono::high_resolution_clock::now();

    if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count() >= m_timeForQuestion)
    {

        if (m_players[user].correctAnswerCount + m_players[user].wrongAnswerCount <= m_players[user].currentQuestionId)
        {
            m_players[user].wrongAnswerCount++;
        }

        m_players[user].currentQuestionId++;

        m_players[user].startTime = currentTime;
        
        if (m_players[user].currentQuestionId >= m_numOfQuestions)
        {
            Question question;
            return question;
        }
        else
        {
            m_players[user].currentQuestion = m_questions[m_players[user].currentQuestionId];
        }
    }

    return m_players[user].currentQuestion;
}

void Game::submitAnswer(const LoggedUser& user, const int answerId)
{
    auto startTime = m_players[user].startTime;
    auto currentTime = std::chrono::high_resolution_clock::now();

    auto timePassed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

    m_players[user].avarageAnswerTime += timePassed;

    if (m_players[user].currentQuestion.getCorrectAnswerId() == answerId)
    {
        m_players[user].correctAnswerCount++;
    }
    else
    {
        m_players[user].wrongAnswerCount++;
    }
}

void Game::removeUser(const LoggedUser& user, const std::weak_ptr<IDatabase>& db)
{
    submitGameStatsToDB(user, db);

    m_players.erase(user);
}

int Game::getGameId() const
{
    return this->m_gameId;
}

std::map<LoggedUser, GameData> Game::getPlayers() const
{
    return m_players;
}

int Game::getNumOfQuestions() const
{
    return m_numOfQuestions;
}

void Game::submitGameStatsToDB(const LoggedUser& user, const std::weak_ptr<IDatabase>& db)
{
    db.lock()->submitGameStatistics(user, m_players[user], calculateScore(m_players[user], user, db));
}

int Game::calculateScore(const GameData& data, const LoggedUser& user, const std::weak_ptr<IDatabase>& db) const
{
    auto database = db.lock();

    float avavarageAnswerTime = (data.avarageAnswerTime / m_numOfQuestions);
    int numOfCorrectAnswers = data.correctAnswerCount;
    int numOfTotalAnswers = data.currentQuestionId;
    int numOfPlayerGames = 1;
    int numOfWrongAnswers = data.wrongAnswerCount;

    int score = ((numOfTotalAnswers + numOfPlayerGames) / ((numOfWrongAnswers + 1) * avavarageAnswerTime)) * 10; // added 1 in case numOfWrongAnswers is 0

    return score;

}
