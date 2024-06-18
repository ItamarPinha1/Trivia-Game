#include "RoomManager.h"

Menu_Handler_Statuses RoomManager::createRoom(const LoggedUser& user, const RoomData& data)
{
    if (doesRoomExistUsingName(data.name))
    {
        return Menu_Handler_Statuses::ROOM_ALREADY_EXISTS;
    }

    std::unique_lock lock(roomMutex_);
    m_rooms[data.id] = Room(user, data);

    return Menu_Handler_Statuses::CREATE_ROOM_SUCCESS;
}

Menu_Handler_Statuses RoomManager::deleteRoom(const int ID)
{
    if (doesRoomExist(ID))
    {
        std::unique_lock lock(roomMutex_);
        m_rooms.erase(m_rooms.find(ID));
        return Menu_Handler_Statuses::ROOM_DELETED_SUCCESSFULLY;
    }
    else 
    {
        return Menu_Handler_Statuses::ROOM_NOT_FOUND;
    }
}

RoomState RoomManager::getRoomState(const int ID) const
{
    auto it = m_rooms.find(ID);
    if (doesRoomExist(ID))
    {
        std::shared_lock lock(roomMutex_);
        it->second.getRoomState();
    }
    else
    {
        return RoomState::DOESNT_EXIST;
    }
}

std::vector<RoomData> RoomManager::getRooms() const
{
    std::vector<RoomData> roomsData;

    for (const auto& roomPair : m_rooms)
    {
        std::unique_lock lock(roomMutex_);
        roomsData.push_back(roomPair.second.getRoomData());
    }

    return roomsData;
}

std::optional<std::reference_wrapper<Room>> RoomManager::getRoom(const int ID)
{
    if (doesRoomExist(ID))
    {
        return m_rooms[ID];
    }
    else
    {
        return std::nullopt;
    }
}

int RoomManager::getLastRoomId() const
{
    int lastId = 0;

    for (const auto& roomPair : m_rooms)
    {
        std::shared_lock lock(roomMutex_);
        if (roomPair.first > lastId)
        {
            lastId = roomPair.first;
        }
    }

    return lastId;
}


bool RoomManager::doesRoomExist(const int ID) const
{
    std::shared_lock lock(roomMutex_);
    return m_rooms.find(ID) != m_rooms.end();
}

bool RoomManager::doesRoomExistUsingName(const std::string& name) const
{
    for (const auto& roomPair : m_rooms)
    {
        std::shared_lock lock(roomMutex_);
        if (roomPair.second.getRoomData().name == name)
        {
            return true;
        }
    }

    return false;
}
