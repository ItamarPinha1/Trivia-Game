#pragma once

#include "IDatabase.h"
#include <memory>

class StatisticsManager
{
public:
	StatisticsManager(const std::weak_ptr<IDatabase> database);

	std::vector<HighScores> getHighScore() const;
	UserStatistics getUserStatistics(const std::string& username) const;

private:
	std::weak_ptr<IDatabase> m_database;
};

