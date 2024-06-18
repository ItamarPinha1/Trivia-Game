#include "JsonResponsePacketSerializer.h"
#include <fstream>
#include "json.hpp"

using std::string;
using json = nlohmann::json;

void to_json(json& j, const RoomData& r) {
    j = json{
        {"id", r.id},
        {"name", r.name},
        {"maxPlayers", r.maxPlayers},
        {"numOfQuestionsInGame", r.numOfQuestionsInGame},
        {"timePerQuestion", r.timePerQuestion},
        {"status", r.status}
    };
}

void to_json(json& j, const HighScores& r) {
    j = json{
        {"score", r.score},
        {"username", r.username}
    };
}

void to_json(json& j, const UserStatistics& r) {
    j = json{
        {"avarageAnswerTime", r.avarageAnswerTime},
        {"numOfCorrectAnswers", r.numOfCorrectAnswers},
        {"numOfPlayerGames", r.numOfPlayerGames},
        {"numOfTotalAnswers", r.numOfTotalAnswers},
        {"playerScore", r.playerScore}
    };
}

void to_json(json& j, const PlayerResults& r) {
    j = json{
        {"username", r.username},
        {"correctAnswerCount", r.correctAnswerCount},
        {"wrongAnswerCount", r.wrongAnswerCount},
        {"averageAnswerTime", r.averageAnswerTime}
    };
}


std::vector<char> JsonResponsePacketSerializer::serializeErrorResponse(const ErrorResponse& error)
{
    json j = json{ {"message", error.message} };
    std::string serializedJson = j.dump();

    

    return addCodeLen(MessageType::MT_ERROR, serializedJson);

}

std::vector<char> JsonResponsePacketSerializer::serializeLoginResponse(const LoginResponse& login)
{
    json j = json{ {"status", login.status} };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_LOG_IN, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeSignUpResponse(const SignupResponse& signUp)
{
    json j = json{ {"status", signUp.status} };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_SIGN_UP, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeLogoutResponse(const LogoutResponse& response)
{
    json j = json{ {"status", response.status} };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_LOG_OUT, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeGetRoomsResponse(const GetRoomsResponse& response)
{
    json j = json{ {"status", response.status}, {"rooms", response.rooms} };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_GET_ROOMS, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeGetPlayersInRoomResponse(const GetPlayersInRoomResponse& response)
{
    json j = json{ {"players", response.players} };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_GET_PLAYERS_IN_ROOM, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeGetHighScoreResponse(const GetHighScoreResponse& response)
{
    json j = json{ {"status", response.status}, {"statistics", response.statistics} };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_HIGH_SCORE, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeGetPersonalStatsResponse(const GetPersonalStatsResponse& response)
{
    json j = json{ {"status", response.status}, {"statistics", response.statistics} };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_GET_STATISTICS, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeJoinRoomResponse(const JoinRoomResponse& response)
{
    json j = json{ {"status", response.status} };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_JOIN_ROOM, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeCreateRoomResponse(const CreateRoomResponse& response)
{
    json j = json{ {"status", response.status} };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_CREATE_ROOM, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeCloseRoomResponse(const CloseRoomResponse& response)
{
    json j = json{ {"status", response.status} };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_CLOSE_ROOM, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeStartGameResponse(const StartGameResponse& response)
{
    json j = json{ {"status", response.status} };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_START_GAME, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeGetRoomStateResponse(const GetRoomStateResponse& response)
{
    json j = {
        {"status", response.status},
        {"roomState", (int)response.roomState},
        {"players", response.players},
        {"questionCount", response.questionCount},
        {"answerTimeout", response.answerTimeout}
    };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_GET_ROOM_STATE, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeLeaveRoomResponse(const LeaveRoomResponse& response)
{
    json j = json{ {"status", response.status} };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_LEAVE_ROOM, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeGetGameResultsResponse(const GetGameResultsResponse& response)
{
    json j = {
        {"status", response.status},
        {"results", response.results}
    };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_GET_GAME_RESULTS, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeSubmitAnswerResponse(const SubmitAnswerResponse& response)
{
    json j = {
        {"status", response.status},
        {"correctAnswerId", response.correctAnswerId}
    };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_SUBMIT_ANSWER, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeGetQuestionResponse(const GetQuestionResponse& response)
{
    json j = {
        {"status", response.status},
        {"question", response.question},
        {"answers", response.answers}
    };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_GET_QUESTION, serializedJson);
}

std::vector<char> JsonResponsePacketSerializer::serializeLeaveGameResponse(const LeaveGameResponse& response)
{
    json j = {
        {"status", response.status}
    };
    std::string serializedJson = j.dump();

    return addCodeLen(MessageType::MT_CLIENT_LEAVE_GAME, serializedJson);
}






std::vector<char> JsonResponsePacketSerializer::addCodeLen(const MessageType& id, const string& data)
{
    std::vector<char> CodeVec = { static_cast<char>(id) };

    std::uint32_t bufferLength = static_cast<std::uint32_t>(data.size());

    std::vector<char> lengthVec(4);
    lengthVec[3] = (bufferLength >> 24) & 0xFF;
    lengthVec[2] = (bufferLength >> 16) & 0xFF;
    lengthVec[1] = (bufferLength >> 8) & 0xFF;
    lengthVec[0] = bufferLength & 0xFF;

    std::vector<char> buffer;
    buffer.reserve(5 + bufferLength);
    buffer.insert(buffer.end(), CodeVec.begin(), CodeVec.end());
    buffer.insert(buffer.end(), lengthVec.begin(), lengthVec.end());
    buffer.insert(buffer.end(), data.begin(), data.end());

    return buffer;
}


