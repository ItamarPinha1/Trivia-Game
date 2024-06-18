#pragma once

#include "IDatabase.h"
#include "LoggedUser.h"
#include "Helper.h"

#include <memory>
#include <vector>
#include <shared_mutex>

class LoginManager
{
public:

	LoginManager(std::weak_ptr<IDatabase> database);

	Login_Handler_Statuses signup(const std::string& username, const std::string& password, const std::string& email);
	Login_Handler_Statuses login(const std::string& username, const std::string& password);
	Menu_Handler_Statuses logout(const std::string& username);

	bool doesUserExist(const std::string& username) const;

private:

	mutable std::shared_mutex loggedUsers_mtx;
	std::weak_ptr<IDatabase> m_database;
	std::vector<LoggedUser> m_loggedUsers;

};

