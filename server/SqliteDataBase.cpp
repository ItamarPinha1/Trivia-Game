#include "SqliteDatabase.h"

#include <iostream>

#include <shared_mutex>

using std::string;

bool SqliteDatabase::open()
{
    std::string DBFileName = "triviaDB.sqlite";
    int file_exist = _access(DBFileName.c_str(), 0);

    std::shared_lock lock1(DatabaseMutex_);
    int res = sqlite3_open(DBFileName.c_str(), &m_db);
    lock1.unlock();

    if (res != SQLITE_OK) {
        m_db = nullptr;
        std::cout << "Failed to open DB" << std::endl;
        return false;
    }
    
    std::unique_lock lock2(DatabaseMutex_);
    if (file_exist != 0)
    {
        const char* sqlStatement = "CREATE TABLE 'USERS' ('NAME' TEXT NOT NULL, 'PASSWORD' TEXT NOT NULL, 'EMAIL' TEXT NOT NULL, PRIMARY KEY('NAME')); ";
        char* errMessage = nullptr;
        res = sqlite3_exec(m_db, sqlStatement, nullptr, nullptr, &errMessage);
        if (res != SQLITE_OK)
            return false;

        // STATISTIC table
        sqlStatement = "CREATE TABLE 'STATISTICS' ('id'	INTEGER UNIQUE,'name'	TEXT NOT NULL UNIQUE,'answered'	INTEGER NOT NULL,'answered_correctly'	INTEGER NOT NULL,'avarage'	INTEGER NOT NULL,'games'	INTEGER NOT NULL,'score'	INTEGER NOT NULL,PRIMARY KEY('id' AUTOINCREMENT))";
        errMessage = nullptr;
        res = sqlite3_exec(m_db, sqlStatement, nullptr, nullptr, &errMessage);
        if (res != SQLITE_OK)
            return false;
        
        sqlStatement = "CREATE TABLE 'QUESTIONS' ('QUESTION' TEXT NOT NULL, 'DIFFICULTY' TEXT NOT NULL, 'CATEGORY' TEXT NOT NULL, 'CORRECT-ANSWER' TEXT NOT NULL, 'ANSWER-A' TEXT NOT NULL,'ANSWER-B' TEXT NOT NULL,'ANSWER-C' TEXT NOT NULL, PRIMARY KEY('QUESTION')); ";
        errMessage = nullptr;
        res = sqlite3_exec(m_db, sqlStatement, nullptr, nullptr, &errMessage);
        if (res != SQLITE_OK)
            return false;
    }
    return true;
}

bool SqliteDatabase::close()
{
    std::unique_lock lock(DatabaseMutex_);
    sqlite3_close(m_db);
    m_db = nullptr;
    return true;
}


SqliteDatabase::SqliteDatabase()
{
    open();
}

SqliteDatabase::~SqliteDatabase()
{
    close();
}

//callbacks
std::list<std::string> SqliteDatabase::s_userNamesList;
std::map< std::string, std::string> SqliteDatabase::s_userNamesAndPasswordsMap;


int SqliteDatabase::getUserNamesCallback(void* data, int argc, char** argv, char** azColName)
{

    SqliteDatabase::s_userNamesList.push_back(argv[0]);
    return 0;
}

int SqliteDatabase::getUserNamesAndPasswordsCallback(void* data, int argc, char** argv, char** azColName)
{
    SqliteDatabase::s_userNamesAndPasswordsMap[argv[0]] = argv[1];
    return 0;
}

int SqliteDatabase::getFloat(void* data, int argc, char** argv, char** azColName)
{
    float* num = (float*)data;

    for (int i = 0; i < argc; i++)
    {
        if (string(azColName[i]) == "avarage")
        {
            *num = std::stof(argv[i]);
        }
    }

    return 0;
}

int SqliteDatabase::getInt(void* data, int argc, char** argv, char** azColName)
{
    int* num = (int*)data;

    for (int i = 0; i < argc; i++)
    {
        if (string(azColName[i]) == "answered_correctly")
        {
            *num = std::stoi(argv[i]);
        }
        else if (string(azColName[i]) == "answered")
        {
            *num = std::stoi(argv[i]);
        }
        else if (string(azColName[i]) == "games")
        {
            *num = std::stoi(argv[i]);
        }
        else if (string(azColName[i]) == "score")
        {
            *num = std::stoi(argv[i]);
        }
    }

    return 0;
}

int SqliteDatabase::getVectorOfHighScores(void* data, int argc, char** argv, char** azColName)
{
    std::vector<HighScores>* highScoresVec = static_cast<std::vector<HighScores>*>(data);

    std::string username;
    int score = 0;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(azColName[i], "name") == 0)
        {
            username = argv[i];
        }
        else if (strcmp(azColName[i], "score") == 0)
        {
            score = std::stoi(argv[i]);
        }
    }

    highScoresVec->push_back({ username, score });

    return 0;
}

int SqliteDatabase::getVectorOfQuestions(void* data, int argc, char** argv, char** azColName)
{
    std::vector<Question>* questions = static_cast<std::vector<Question>*>(data);

    std::string question, correctAns;
    std::vector<std::string> answers;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(azColName[i], "QUESTION") == 0)
        {
            question = argv[i];
        }
        else if (strcmp(azColName[i], "CORRECT-ANSWER") == 0)
        {
            correctAns = argv[i];
            answers.push_back(argv[i]);
        }
        else if (strcmp(azColName[i], "ANSWER-A") == 0 || strcmp(azColName[i], "ANSWER-B") == 0 || strcmp(azColName[i], "ANSWER-C") == 0)
        {
            answers.push_back(argv[i]);
        }
    }

    questions->push_back(Question(question, correctAns, answers));

    return 0;
}



bool SqliteDatabase::doesUserExist(const std::string& username) const
{
    const char* sqlStatement = "SELECT NAME FROM USERS ;";
    char* errMessage = nullptr;
    
    std::shared_lock lock1(DatabaseMutex_);
    int res = sqlite3_exec(m_db, sqlStatement, SqliteDatabase::getUserNamesCallback, nullptr, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "error running the sqlite3_exec" << std::endl;
        return false;
    }
    for (auto& user_name : SqliteDatabase::s_userNamesList)
    {
        if (user_name == username)
        {
            std::unique_lock lock2(userNamesListMutex_);
            SqliteDatabase::s_userNamesList.clear();
            return true;
        }
    }
    std::unique_lock lock2(userNamesListMutex_);
    SqliteDatabase::s_userNamesList.clear();
    return false;
}

bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password) const
{
    const char* sqlStatement = "SELECT NAME, PASSWORD FROM USERS ;";
    char* errMessage = nullptr;
    
    std::shared_lock lock1(DatabaseMutex_);
    int res = sqlite3_exec(m_db, sqlStatement, SqliteDatabase::getUserNamesAndPasswordsCallback, nullptr, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "error running the sqlite3_exec" << std::endl;
        return false;
    }

    std::unique_lock lock2(userNamesAndPasswordsMapMutex_);

    if (SqliteDatabase::s_userNamesAndPasswordsMap[username] == password)
    {
        SqliteDatabase::s_userNamesAndPasswordsMap.clear();
        return true;
    }
    SqliteDatabase::s_userNamesAndPasswordsMap.clear();
    return false;
}

void SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{

    std::string sqlStatementStr = "INSERT INTO USERS (NAME, PASSWORD, EMAIL) VALUES ('" + username + "', '" + password + "', '" + email + "');";
    const char* sqlStatement = sqlStatementStr.c_str();
    char* errMessage = nullptr;
    
    std::unique_lock lock(DatabaseMutex_);
    int res = sqlite3_exec(m_db, sqlStatement, nullptr, nullptr, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "error running the sqlite3_exec" << std::endl;
        return;
    }
}

float SqliteDatabase::getPlayerAverageAnswerTime(const std::string& username) const
{
    float avarage = 0;


    std::string sqlStatementStr = "SELECT avarage From STATISTICS WHERE name = '" + username + "'; ";
    const char* sqlStatement = sqlStatementStr.c_str();
    char* errMessage = nullptr;
    
    std::shared_lock lock(DatabaseMutex_);
    int res = sqlite3_exec(m_db, sqlStatement, getFloat, &avarage, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "error running the sqlite3_exec" << std::endl;
        return 0;
    }

    return avarage;
}

int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username) const
{
    int correctAnswers = 0;


    std::string sqlStatementStr = "SELECT answered_correctly From STATISTICS WHERE name = '" + username + "'; ";
    const char* sqlStatement = sqlStatementStr.c_str();
    char* errMessage = nullptr;
    
    std::shared_lock lock(DatabaseMutex_);
    int res = sqlite3_exec(m_db, sqlStatement, getInt, &correctAnswers, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "error running the sqlite3_exec" << std::endl;
        return 0;
    }

    return correctAnswers;
}

int SqliteDatabase::getNumOfTotalAnswers(const std::string& username) const
{

    int totalAnswers = 0;

    std::string sqlStatementStr = "SELECT answered From STATISTICS WHERE name = '" + username + "'; ";
    const char* sqlStatement = sqlStatementStr.c_str();
    char* errMessage = nullptr;
    
    std::shared_lock lock(DatabaseMutex_);
    int res = sqlite3_exec(m_db, sqlStatement, getInt, &totalAnswers, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "error running the sqlite3_exec" << std::endl;
        return 0;
    }

    return totalAnswers;
}

int SqliteDatabase::getNumOfPlayerGames(const std::string& username) const
{
    int games = 0;


    std::string sqlStatementStr = "SELECT games From STATISTICS WHERE name = '" + username + "'; ";
    const char* sqlStatement = sqlStatementStr.c_str();
    char* errMessage = nullptr;
    
    std::shared_lock lock(DatabaseMutex_);
    int res = sqlite3_exec(m_db, sqlStatement, getInt, &games, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "error running the sqlite3_exec" << std::endl;
        return 0;
    }

    return games;
}

int SqliteDatabase::getPlayerScore(const std::string& username) const
{
    int score = 0;


    std::string sqlStatementStr = "SELECT score From STATISTICS WHERE name = '" + username + "'; ";
    const char* sqlStatement = sqlStatementStr.c_str();
    char* errMessage = nullptr;
    
    std::shared_lock lock(DatabaseMutex_);
    int res = sqlite3_exec(m_db, sqlStatement, getInt, &score, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "error running the sqlite3_exec" << std::endl;
        return 0;
    }

    return score;
}

std::vector<HighScores> SqliteDatabase::getHighScores() const
{
    std::vector<HighScores> highScores;


    std::string sqlStatementStr = "SELECT name, score FROM STATISTICS ORDER BY score DESC LIMIT 5; ";
    const char* sqlStatement = sqlStatementStr.c_str();
    char* errMessage = nullptr;
    
    std::shared_lock lock(DatabaseMutex_);
    int res = sqlite3_exec(m_db, sqlStatement, getVectorOfHighScores, &highScores, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "error running the sqlite3_exec" << std::endl;
        return {};
    }

    return highScores;
}

std::vector<Question> SqliteDatabase::getQuestions(const int num) const
{
    std::vector<Question> q;
    

    std::string sqlStatementStr = "SELECT * FROM QUESTIONS LIMIT " + std::to_string(num) + ";";
    const char* sqlStatement = sqlStatementStr.c_str();
    char* errMessage = nullptr;
    
    std::shared_lock lock(DatabaseMutex_);
    int res = sqlite3_exec(m_db, sqlStatement, getVectorOfQuestions, &q, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "error running the sqlite3_exec" << std::endl;
        return {};
    }

    return q;

}

void SqliteDatabase::submitGameStatistics(const LoggedUser& user, const GameData& data, int score) const
{
    int numOfCorrectAnswers = getNumOfCorrectAnswers(user.getUserName()) + data.correctAnswerCount;
    int numOfTotalAnswers = getNumOfTotalAnswers(user.getUserName()) + data.currentQuestionId - 1; // substructed 1 because the id is 1 above the actual number of questions
    int numOfPlayerGames = getNumOfPlayerGames(user.getUserName()) + 1;
    float avavarageAnswerTime = (getPlayerAverageAnswerTime(user.getUserName()) + data.avarageAnswerTime) / 2;
    score += getPlayerScore(user.getUserName());


    std::string sqlStatementStr = "INSERT OR REPLACE INTO STATISTICS (name, answered, answered_correctly, avarage, games, score) VALUES('" + user.getUserName() + "', " + std::to_string(numOfTotalAnswers) + ", " + std::to_string(numOfCorrectAnswers) + ", " + std::to_string(numOfCorrectAnswers) + ", " + std::to_string(numOfPlayerGames) + ", " + std::to_string(score) + ");";
    const char* sqlStatement = sqlStatementStr.c_str();
    char* errMessage = nullptr;

    std::shared_lock lock(DatabaseMutex_);
    int res = sqlite3_exec(m_db, sqlStatement, nullptr, nullptr, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "error running the sqlite3_exec" << std::endl;
    }
}


