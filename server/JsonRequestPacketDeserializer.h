#pragma once

#include <vector>
#include <string>
#include "Helper.h"


struct LoginRequest
{
	std::string username;
	std::string password;
};

struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
};

struct GetPlayersInRoomRequest
{
	unsigned int roomId;
};

struct JoinRoomRequest
{
	unsigned int roomId;
};

struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
	std::string category;
	std::string difficulty;
};

struct SubmitAnswerRequest
{
	int answerId;
};

class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(const std::vector<char>& buffer);
	static SignupRequest deserializeSignUpRequest(const std::vector<char>& buffer);
	static GetPlayersInRoomRequest deserializeGetPlayersRequest(const std::vector<char>& buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(const std::vector<char>& buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(const std::vector<char>& buffer);

	static SubmitAnswerRequest deserializeSubmitAnswerRequest(const std::vector<char>& buffer);
};

