#pragma once

#include <vector>
#include <string>
#include <WinSock2.h>
#include <iostream>


enum MessageType : byte
{
	MT_ERROR = 0,
	MT_CLIENT_LOG_IN = 10,
	MT_CLIENT_SIGN_UP = 20,

	MT_CLIENT_LOG_OUT = 30,
	MT_CLIENT_CREATE_ROOM = 40,
	MT_CLIENT_GET_ROOMS = 50,
	MT_CLIENT_GET_PLAYERS_IN_ROOM = 60,
	MT_CLIENT_JOIN_ROOM = 70,
	MT_CLIENT_GET_STATISTICS = 80,
	MT_CLIENT_HIGH_SCORE = 90,

	MT_CLIENT_CLOSE_ROOM = 100,
	MT_CLIENT_START_GAME = 110,
	MT_CLIENT_GET_ROOM_STATE = 120,
	MT_CLIENT_LEAVE_ROOM = 130,
	
	MT_CLIENT_LEAVE_GAME = 140,
	MT_CLIENT_GET_QUESTION = 150,
	MT_CLIENT_SUBMIT_ANSWER = 160,
	MT_CLIENT_GET_GAME_RESULTS = 170,

};

enum countOfBytes : int
{
	CODE_BYTE_COUNT = 1,
	DATA_SIZE_BYTE_COUNT = 4
};

enum class Login_Handler_Statuses : unsigned int
{
	LOGIN_SUCCES = 11,
	LOGIN_FAIL = 12,
	USER_DOESNT_EXIST = 13,
	PASSWORD_DOESNT_MATCH = 14,
	USER_ALREADY_LOGGED_IN = 15,

	SIGN_UP_SUCCES = 21,
	SIGN_UP_FAIL = 22,
	USER_EXIST = 23,
};

enum class Menu_Handler_Statuses : unsigned int
{
	LOGOUT_SUCCES = 31,
	LOGGOUT_FAIL = 32,
	USER_ISNT_LOGGED_IN = 33,

	CREATE_ROOM_SUCCESS = 41,
	UNKNOWN_ERROR = 42,
	ROOM_NOT_FOUND = 43,
	ROOM_ALREADY_EXISTS = 44,
	ROOM_DELETED_SUCCESSFULLY = 45,
	USER_ALREADY_IN_ROOM = 46,
	ROOM_IS_FULL = 47,

	GET_ROOMS_SUCCESS = 51,
	GET_ROOMS_FAIL = 52,

	GET_PLAYERS_IN_ROOM_SUCCESS = 62,
	GET_PLAYERS_IN_ROOM_FAIL = 63,

	JOIN_ROOM_SUCCESS = 71,
	JOIN_ROOM_FAIL = 72,

	GET_STATISTICS_SUCCESS = 81,
	GET_STATISTICS_FAIL = 82,
	USER_DOESNT_EXIST = 83,

	GET_HIGH_SCORES_SUCCESS = 91,
	GET_HIGH_SCORES_FAIL = 92,
};

enum class In_Room_Statueses : int
{
	CLOSE_ROOM_SUCCES = 101,
	CLOSE_ROOM_FAIL = 102,

	START_GAME_SUCCES = 111,
	START_GAME_FAIL = 112,

	GET_ROOM_STATE_SUCCES = 121,
	GET_ROOM_STATE_FAIL = 122,

	LEAVE_ROOM_SUCCES = 131,
	LEAVE_ROOM_FAIL = 132,
};

enum class Game_Statueses : int
{
	LEAVE_SUCESS = 141,
	LEAVE_FAIL = 142,

	GET_QUESTION_SUCCES = 151,
	GET_QUESTION_FAIL = 152,
	GAME_FINISHED = 153,

	SUBMIT_ANSWER_SUCCES = 161,
	SUBMIT_ANSWER_FAIL = 162,

	GET_GAME_RESULTS_SUCCES = 171,
	GET_GAME_RESULTS_FAIL = 172,
};

class Helper
{
public:


	static std::vector<char> getMessageTypeCodeByte(const SOCKET sc);
	static int getLenPartFromSocket(const SOCKET sc, const int bytesNum);
	static std::vector<char> getJsonPartFromSocket(SOCKET sc, const int bytesNum);
	static void sendData(const SOCKET sc, const std::vector<char> message);
	static std::string getPaddedNumber(const int num, const int digits);

	static int buffToInteger(std::vector<char> buffer, const int numBytes);

private:
	static std::vector<char> getPartFromSocket(const SOCKET sc, const int bytesNum);
	static std::vector<char> getPartFromSocket(const SOCKET sc, const int bytesNum, const int flags);

};


#ifdef _DEBUG // vs add this define in debug mode
#include <stdio.h>
// Q: why do we need traces ?
// A: traces are a nice and easy way to detect bugs without even debugging
// or to understand what happened in case we miss the bug in the first time
#define TRACE(msg, ...) printf(msg "\n", __VA_ARGS__);
// for convenient reasons we did the traces in stdout
// at general we would do this in the error stream like that
// #define TRACE(msg, ...) fprintf(stderr, msg "\n", __VA_ARGS__);

#else // we want nothing to be printed in release version
#define TRACE(msg, ...) printf(msg "\n", __VA_ARGS__);
#define TRACE(msg, ...) // do nothing
#endif