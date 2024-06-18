#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <io.h>
#include <list>
#include <map>
#include <shared_mutex>
#include <string>

#include "Question.h"

#include <shared_mutex>

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase();
	~SqliteDatabase();

	bool open() override;
	bool close() override;
	bool doesUserExist(const std::string& username) const override;
	bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	void addNewUser(const std::string& username, const std::string& password, const std::string& email) override;

	float getPlayerAverageAnswerTime(const std::string& username) const override;
	int getNumOfCorrectAnswers(const std::string& username) const override;
	int getNumOfTotalAnswers(const std::string& username) const override;
	int getNumOfPlayerGames(const std::string& username) const override;
	int getPlayerScore(const std::string& username) const override;
	std::vector<HighScores> getHighScores() const override;

	std::vector<Question> getQuestions(const int num) const override;

	void submitGameStatistics(const LoggedUser& user, const GameData& data, const int score) const override;


	//callbacks
	static int getUserNamesCallback(void* data, int argc, char** argv, char** azColName);
	static int getUserNamesAndPasswordsCallback(void* data, int argc, char** argv, char** azColName);
	static int getFloat(void* data, int argc, char** argv, char** azColName);
	static int getInt(void* data, int argc, char** argv, char** azColName);
	static int getVectorOfHighScores(void* data, int argc, char** argv, char** azColName);
	static int getVectorOfQuestions(void* data, int argc, char** argv, char** azColName);



private:
	sqlite3* m_db;

	mutable std::shared_mutex DatabaseMutex_;
	mutable std::shared_mutex userNamesListMutex_;
	mutable std::shared_mutex userNamesAndPasswordsMapMutex_;

	//callbacks
	static std::list<std::string> s_userNamesList;
	static std::map< std::string, std::string> s_userNamesAndPasswordsMap;
};

