#include "LoggedUser.h"

LoggedUser::LoggedUser(const std::string& userName) : m_username(userName)
{
}

std::string LoggedUser::getUserName() const
{
    return this->m_username;
}

bool LoggedUser::operator==(const LoggedUser& other) const
{
    return m_username == other.getUserName();
}

bool LoggedUser::operator<(const LoggedUser& other) const
{
    return m_username < other.getUserName();
}
