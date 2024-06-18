#include "JsonRequestPacketDeserializer.h"

#include <fstream>
#include "json.hpp"

using std::string;
using json = nlohmann::json;


LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const std::vector<char>& buffer)
{
    LoginRequest lRequest;

    json j = json::parse(buffer);

    lRequest.username = j["username"];
    lRequest.password = j["password"];

    return lRequest;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignUpRequest(const std::vector<char>& buffer)
{
    SignupRequest sRequest;

    json j = json::parse(buffer);

    sRequest.username = j["username"];
    sRequest.password = j["password"];
    sRequest.email = j["email"];

    return sRequest;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(const std::vector<char>& buffer)
{
    GetPlayersInRoomRequest pRequest;

    json j = json::parse(buffer);

    pRequest.roomId = j["roomId"];

    return pRequest;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const std::vector<char>& buffer)
{
    JoinRoomRequest jRequest;

    json j = json::parse(buffer);

    jRequest.roomId = j["roomId"];

    return jRequest;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const std::vector<char>& buffer)
{
    CreateRoomRequest cRequest;

    json j = json::parse(buffer);

    cRequest.roomName = j["roomName"];
    cRequest.maxUsers = j["maxUsers"];
    cRequest.questionCount = j["questionCount"];
    cRequest.answerTimeout = j["answerTimeout"];
    

    return cRequest;
}

SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(const std::vector<char>& buffer)
{
    SubmitAnswerRequest cRequest;

    std::vector<char> newBuffer(buffer.begin(), buffer.begin() + 14); // sometimes get invalid json struct

    json j = json::parse(newBuffer);

    cRequest.answerId = j["answerId"];

    return cRequest;
}
