#pragma once

#include <string>

class LoggedUser
{
public:
	LoggedUser(const std::string& userName);

	std::string getUserName() const;

	bool operator==(const LoggedUser& other) const;
	bool operator<(const LoggedUser& other) const;

private:
	std::string m_username;
};

