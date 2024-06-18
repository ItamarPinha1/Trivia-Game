#include "GameManager.h"
#include <algorithm>

GameManager::GameManager(const std::weak_ptr<IDatabase>& db) : m_database(db), m_games({})
{

}

Game& GameManager::createGame(const Room& room, const int gameId)
{
    std::vector<Question> questions = m_database.lock()->getQuestions(room.getRoomData().numOfQuestionsInGame);
    
    std::map<LoggedUser, GameData> players;
    for (auto& player : room.getAllUser())  
    {
        players[LoggedUser(player)] = GameData();
    }

    m_games.push_back(Game(questions, players, gameId, room.getRoomData().timePerQuestion, room.getRoomData().numOfQuestionsInGame));

    return m_games.back();
}

void GameManager::deleteGame(const int gameId)
{
    // Remove the game
    m_games.erase(std::remove_if(m_games.begin(), m_games.end(), [gameId](const Game& game) {
        return game.getGameId() == gameId;
        }), m_games.end());
}

std::optional<std::reference_wrapper<Game>> GameManager::getGame(const int gameId)
{
    for (auto& game : m_games)
    {
        if (game.getGameId() == gameId)
        {
            return game;
        }
    }

    return std::nullopt;
}

std::vector<PlayerResults> GameManager::getGameResults(const int gameId)
{
    auto game = getGame(gameId);

    if (game)
    {
        PlayerResults result;
        std::vector<PlayerResults> resultVector;

        for (auto& player : game->get().getPlayers())
        {
            result.username = player.first.getUserName();

            result.averageAnswerTime = player.second.avarageAnswerTime / game->get().getNumOfQuestions();
            result.correctAnswerCount = player.second.correctAnswerCount;
            result.wrongAnswerCount = player.second.wrongAnswerCount;
            result.score = game->get().calculateScore(player.second, player.first, m_database);

            resultVector.push_back(result);
        }
        std::sort(resultVector.begin(), resultVector.end(), [](const PlayerResults& playerA, const PlayerResults& playerB) {
            return playerA.score > playerB.score;});

        return resultVector;

    }

    return {};
}


