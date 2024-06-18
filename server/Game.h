#pragma once

#include "Question.h"
#include "map"
#include "LoggedUser.h"
#include <memory>
#include "IDatabase.h"

#include <chrono>

class IDatabase;


struct GameData
{
	Question currentQuestion;
	int correctAnswerCount;
	int wrongAnswerCount;
	int avarageAnswerTime;

	int currentQuestionId;
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};


class Game
{
public:
	Game(const std::vector<Question>& questions, const std::map<LoggedUser, GameData>& players, const int id, const int timeForQuestin, const int numOfQuestions);

	Question getQuestionForUser(const LoggedUser& user);
	void submitAnswer(const LoggedUser& user, const int answerId);
	void removeUser(const LoggedUser& user, const std::weak_ptr<IDatabase>& db);

	int getGameId() const;
	std::map<LoggedUser, GameData> getPlayers() const;
	int getNumOfQuestions() const;

	int calculateScore(const GameData& data, const LoggedUser& user, const std::weak_ptr<IDatabase>& db) const;

private:
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;
	int m_gameId;
	int m_timeForQuestion;
	int m_numOfQuestions;

	void submitGameStatsToDB(const LoggedUser& user, const std::weak_ptr<IDatabase>& db);

	
};

