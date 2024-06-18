#pragma once

#include <vector>
#include <string>
#include "LoggedUser.h"
#include "Helper.h"

enum RoomState
{
	DIDNT_START,
	IN_GAME,
	DOESNT_EXIST,
	ROOM_CLOSE
};

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	RoomState status;
};

class Room
{
public:
	Room() = default;
	Room(const LoggedUser& user, const RoomData& data);
	Menu_Handler_Statuses addUser(const LoggedUser& user);
	In_Room_Statueses removeUser(const LoggedUser& user);
	std::vector<std::string> getAllUser() const;
	RoomState getRoomState() const;
	RoomData getRoomData() const;
	void setRoomState(const RoomState status);
private:
	RoomData m_metaData;
	std::vector<LoggedUser> m_users;
};

