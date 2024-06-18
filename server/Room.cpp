#include "Room.h"
#include <iostream>

Room::Room(const LoggedUser& user, const RoomData& data)
{
    m_metaData.id = data.id;
    m_metaData.maxPlayers = data.maxPlayers;
    m_metaData.name = data.name;
    m_metaData.numOfQuestionsInGame = data.numOfQuestionsInGame;
    m_metaData.status = data.status;
    m_metaData.timePerQuestion = data.timePerQuestion;

    m_users.push_back(user);
}

Menu_Handler_Statuses Room::addUser(const LoggedUser& user)
{
    if (m_users.size() >= m_metaData.maxPlayers) 
    {
        std::cerr << "Cannot add user to room: room is full." << std::endl;
        return Menu_Handler_Statuses::ROOM_IS_FULL;
    }

    if (std::find(m_users.begin(), m_users.end(), user) != m_users.end()) 
    {
        std::cerr << "Cannot add user to room: user is already in the room." << std::endl;
        return Menu_Handler_Statuses::USER_ALREADY_IN_ROOM;
    }

    m_users.push_back(user);
    return Menu_Handler_Statuses::JOIN_ROOM_SUCCESS;
}



In_Room_Statueses Room::removeUser(const LoggedUser& user)
{
    if (m_users.empty()) {
        return In_Room_Statueses::LEAVE_ROOM_FAIL;
    }

    auto it = std::find(m_users.begin(), m_users.end(), user);
    if (it == m_users.end()) {
        return In_Room_Statueses::LEAVE_ROOM_FAIL;
    }

    auto new_logical_end = std::remove_if(m_users.begin(), m_users.end(), [&](const LoggedUser& u) { return u.getUserName() == user.getUserName(); });
    m_users.erase(new_logical_end, m_users.end());

    return In_Room_Statueses::LEAVE_ROOM_SUCCES;

}

std::vector<std::string> Room::getAllUser() const
{
    std::vector<std::string> usernames;
    for (const auto& user : m_users)
    {
        usernames.push_back(user.getUserName());
    }
    return usernames;
}

RoomState Room::getRoomState() const
{
    return m_metaData.status;
}

void Room::setRoomState(const RoomState status)
{
    m_metaData.status = status;
}

RoomData Room::getRoomData() const
{
    return m_metaData;
}
