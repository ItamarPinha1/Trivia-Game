#pragma once
#include "Helper.h"
#include "Room.h"
#include "StatisticsManager.h"

#include <vector>
#include <string>
#include <map>

struct LoginResponse
{
	unsigned int status;
};

struct SignupResponse
{
	unsigned int status;
};

struct ErrorResponse
{
	std::string message;
};

struct LogoutResponse
{
	unsigned int status;
};

struct GetRoomsResponse
{
	unsigned int status;
	std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse
{
	std::vector<std::string> players;
};

struct GetHighScoreResponse
{
	unsigned int status;
	std::vector<HighScores> statistics;
};

struct GetPersonalStatsResponse
{
	unsigned int status;
	UserStatistics statistics;
};

struct JoinRoomResponse
{
	unsigned int status;
};

struct CreateRoomResponse
{
	unsigned int status;
};

struct CloseRoomResponse
{
	unsigned int status;
};

struct StartGameResponse
{
	unsigned int status;
};

struct GetRoomStateResponse
{
	unsigned int status;
	RoomState roomState;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;

};

struct LeaveRoomResponse
{
	unsigned int status;
};

struct LeaveGameResponse
{
	unsigned int status;
};

struct GetQuestionResponse
{
	unsigned int status;
	std::string question;
	std::map<unsigned int, std::string> answers;
};

struct SubmitAnswerResponse
{
	unsigned int status;
	unsigned int correctAnswerId;
};

struct PlayerResults
{
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
	unsigned int score;
};

struct GetGameResultsResponse
{
	unsigned int status;
	std::vector<PlayerResults> results;
};




class JsonResponsePacketSerializer
{
public:


	static std::vector<char> serializeErrorResponse(const ErrorResponse &error);
	static std::vector<char> serializeLoginResponse(const LoginResponse &login);
	static std::vector<char> serializeSignUpResponse(const SignupResponse &signUp);

	static std::vector<char> serializeLogoutResponse(const LogoutResponse& response);
	static std::vector<char> serializeGetRoomsResponse(const GetRoomsResponse& response);
	static std::vector<char> serializeGetPlayersInRoomResponse(const GetPlayersInRoomResponse& response);
	static std::vector<char> serializeGetHighScoreResponse(const GetHighScoreResponse& response);
	static std::vector<char> serializeGetPersonalStatsResponse(const GetPersonalStatsResponse& response);
	static std::vector<char> serializeJoinRoomResponse(const JoinRoomResponse& response);
	static std::vector<char> serializeCreateRoomResponse(const CreateRoomResponse& response);

	static std::vector<char> serializeCloseRoomResponse(const CloseRoomResponse& response);
	static std::vector<char> serializeStartGameResponse(const StartGameResponse& response);
	static std::vector<char> serializeGetRoomStateResponse(const GetRoomStateResponse& response);
	static std::vector<char> serializeLeaveRoomResponse(const LeaveRoomResponse& response);

	static std::vector<char> serializeGetGameResultsResponse(const GetGameResultsResponse& response);
	static std::vector<char> serializeSubmitAnswerResponse(const SubmitAnswerResponse& response);
	static std::vector<char> serializeGetQuestionResponse(const GetQuestionResponse& response);
	static std::vector<char> serializeLeaveGameResponse(const LeaveGameResponse& response);

private:
	static std::vector<char> addCodeLen(const MessageType& id, const std::string& data);
};

