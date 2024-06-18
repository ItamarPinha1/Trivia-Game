#include "LoginManager.h"

#include <iostream>
#include <algorithm>

#include "SHA256.h"
#include <regex>





bool doesPasswordValid(const std::string& password)
{
	std::regex regex1(".{8,}"); // check if has atleast 8 characters
	if (!std::regex_search(password, regex1))
	{
		return false;
	}

	std::regex regex2("[A-Z]"); // check if has atleast 1 upper case letter
	if (!std::regex_search(password, regex2))
	{
		return false;
	}

	std::regex regex3("[a-z]"); // check if has atleast 1 lower case letter
	if (!std::regex_search(password, regex3))
	{
		return false;
	}

	std::regex regex4("[1-9]"); // check if has atleast 1 integer
	if (!std::regex_search(password, regex4))
	{
		return false;
	}

	std::regex regex5("[*&^%$#@!]"); // check if has atleast 1 special character
	if (!std::regex_search(password, regex5))
	{
		return false;
	}

	return true;
}

bool doesEmailValid(const std::string& email)
{
	std::regex regex1("^[a-zA-Z0-9]+"); // check if starts with letters or numbers
	if (!std::regex_search(email, regex1))
	{
		return false;
	}

	std::regex regex2("[.]"); // check if has .
	if (!std::regex_search(email, regex2))
	{
		return false;
	}

	std::regex regex3("@[A-Za-z0-9\.\-]+\.[A-Za-z]{2,}"); // check if domain is valid
	if (!std::regex_search(email, regex3))
	{
		return false;
	}

	return true;
}




LoginManager::LoginManager(std::weak_ptr<IDatabase> database) : m_database(std::move(database)), m_loggedUsers({})
{
	if (!m_database.expired())
	{
		m_database.lock()->open();
	}
	else
	{
		std::cout << "Error with db" << std::endl;
	}
}

Login_Handler_Statuses LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
	SHA256 sha;
	if (!m_database.expired() && doesPasswordValid(password) && doesEmailValid(email))
	{
		if (!m_database.lock()->doesUserExist(username))
		{
			sha.update(password);
			std::array<uint8_t, 32> digest = sha.digest();

			std::string shaPassword = SHA256::toString(digest);

			m_database.lock()->addNewUser(username, shaPassword, email);

			std::unique_lock lock(loggedUsers_mtx); // use mutex
			m_loggedUsers.push_back(LoggedUser(username));

			return Login_Handler_Statuses::SIGN_UP_SUCCES;
		}
		else
		{
			return Login_Handler_Statuses::USER_EXIST;
		}
	}
	else
	{
		std::cout << "Error with db" << std::endl;
		return Login_Handler_Statuses::SIGN_UP_FAIL;
	}
}

Login_Handler_Statuses LoginManager::login(const std::string& username, const std::string& password)
{

	for (const auto& loggedUser : m_loggedUsers)
	{
		std::shared_lock lock(loggedUsers_mtx);
		if (loggedUser.getUserName() == username)
		{
			std::cout << "User is already logged in" << std::endl;
			return Login_Handler_Statuses::USER_ALREADY_LOGGED_IN; // User is already logged in
		}
	}

	if (!m_database.expired())
	{
		if (m_database.lock()->doesUserExist(username))
		{
			SHA256 sha;

			sha.update(password);
			std::array<uint8_t, 32> digest = sha.digest();

			std::string shaPassword = SHA256::toString(digest);


			if (m_database.lock()->doesPasswordMatch(username, shaPassword))
			{
				std::unique_lock lock(loggedUsers_mtx); // use mutex
				m_loggedUsers.push_back(LoggedUser(username));
				return Login_Handler_Statuses::LOGIN_SUCCES;
			}
			else
			{
				std::cout << "Wrong Password" << std::endl;
				return Login_Handler_Statuses::PASSWORD_DOESNT_MATCH;
			}
		}
		else
		{
			std::cout << "User doesnt exist" << std::endl;
			return Login_Handler_Statuses::USER_DOESNT_EXIST;
		}
	}
	else
	{
		std::cout << "Error with db" << std::endl;
		return Login_Handler_Statuses::LOGIN_FAIL;
	}
}

Menu_Handler_Statuses LoginManager::logout(const std::string& username)
{
	if (!m_database.expired())
	{
		std::unique_lock lock(loggedUsers_mtx);
		auto new_logical_end = std::remove_if(m_loggedUsers.begin(), m_loggedUsers.end(), [&](const LoggedUser& user) { return user.getUserName() == username; });
		m_loggedUsers.erase(new_logical_end, m_loggedUsers.end());
		// use algorithm to delete user if it's in the vector
		
		return Menu_Handler_Statuses::LOGOUT_SUCCES;
	}
	else
	{
		std::cout << "Error with db" << std::endl;
		return Menu_Handler_Statuses::LOGGOUT_FAIL;
	}
}

bool LoginManager::doesUserExist(const std::string& username) const
{
	std::shared_lock lock(loggedUsers_mtx);
	return std::find(m_loggedUsers.begin(), m_loggedUsers.end(), username) != m_loggedUsers.end();
}