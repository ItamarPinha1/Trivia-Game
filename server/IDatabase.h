#pragma once
#include <string>
#include <vector>
#include <list>
#include "Question.h"
#include "Game.h"

struct GameData;

struct UserStatistics
{
	float avarageAnswerTime;
	int numOfCorrectAnswers;
	int numOfTotalAnswers;
	int numOfPlayerGames;
	int playerScore;
};


struct HighScores
{
	std::string username;
	int score;
};


class IDatabase
{
public:
	virtual ~IDatabase() = default;
	virtual bool open() = 0;
	virtual bool close() = 0;

	//users
	virtual bool doesUserExist(const std::string& username) const = 0;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const = 0;
	virtual void addNewUser(const std::string& username, const std::string& password, const std::string& email) = 0;

	virtual float getPlayerAverageAnswerTime(const std::string& username) const = 0;
	virtual int getNumOfCorrectAnswers(const std::string& username) const = 0;
	virtual int getNumOfTotalAnswers(const std::string& username) const = 0;
	virtual int getNumOfPlayerGames(const std::string& username) const = 0;
	virtual int getPlayerScore(const std::string& username) const = 0;
	virtual std::vector<HighScores> getHighScores() const = 0;

	virtual std::vector<Question> getQuestions(const int num) const = 0;

	virtual void submitGameStatistics(const LoggedUser& user, const GameData& data, const int score) const = 0;
};

