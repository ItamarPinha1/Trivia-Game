#pragma once

#include <optional>
#include <map>
#include "Room.h"
#include <shared_mutex>

class RoomManager
{
public:
    Menu_Handler_Statuses createRoom(const LoggedUser& user, const RoomData& data);
    Menu_Handler_Statuses deleteRoom(const int ID);
    RoomState getRoomState(const int ID) const;
    std::vector<RoomData> getRooms() const;
    std::optional<std::reference_wrapper<Room>> getRoom(const int ID);
    int getLastRoomId() const;

private:
	std::map<int, Room> m_rooms;

    mutable std::shared_mutex roomMutex_;

    bool doesRoomExist(const int ID) const;
    bool doesRoomExistUsingName(const std::string& name) const;

};

