#include "MenuRequestHandler.h"

#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

MenuRequestHandler::MenuRequestHandler(LoggedUser user, RequestHandlerFactory& handleFactory):
    m_user(user), m_handlerFactory(handleFactory)
{
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
	return request.id == MessageType::MT_CLIENT_LOG_OUT || request.id == MessageType::MT_CLIENT_CREATE_ROOM 
		||request.id == MessageType::MT_CLIENT_GET_ROOMS || request.id == MessageType::MT_CLIENT_GET_PLAYERS_IN_ROOM 
		|| request.id == MessageType::MT_CLIENT_JOIN_ROOM || request.id == MessageType::MT_CLIENT_GET_STATISTICS;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& request)
{
    RequestResult result;
    switch (request.id)
    {
    case MT_CLIENT_LOG_OUT:
    {
        result = signout(request);
        break;
    }
    case MT_CLIENT_CREATE_ROOM:
    {
        result = createRoom(request);
        break;
    }
    case MT_CLIENT_GET_ROOMS:
    {
        result = getRooms(request);
        break;
    }
    case MT_CLIENT_GET_PLAYERS_IN_ROOM:
    {
        result = getPlayersInRoom(request);
        break;
    }
    case MT_CLIENT_JOIN_ROOM:
    {
        result = joinRoom(request);
        break;
    }
    case MT_CLIENT_GET_STATISTICS:
    {
        result = getPersonalStats(request);
        break;
    }
    case MT_CLIENT_HIGH_SCORE:
    {
        result = getHighScore(request);
        break;
    }
    default:
    {
        ErrorResponse errorResponse;
        errorResponse.message = "Invalid request ID";

        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);
        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
        break;
    }
    }
    return result;
}

RequestResult MenuRequestHandler::signout(const RequestInfo& request) const
{
    RequestResult result;
    LogoutResponse logoutResponse;

    try
    {

        logoutResponse.status = (int)m_handlerFactory.getLoginManager().logout(m_user.getUserName());

        result.response = JsonResponsePacketSerializer::serializeLogoutResponse(logoutResponse);

        if (logoutResponse.status == (int)Menu_Handler_Statuses::LOGOUT_SUCCES)
        {
            result.newHandler = m_handlerFactory.createLoginRequestHandler();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;

        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);

    }
    
    return result;
}

RequestResult MenuRequestHandler::getRooms(const RequestInfo& request) const
{
    RequestResult result;
    GetRoomsResponse getRoomsResponse;

    try
    {
        getRoomsResponse.rooms = m_handlerFactory.getRoomManager().getRooms();
        getRoomsResponse.status = static_cast<int>(Menu_Handler_Statuses::GET_ROOMS_SUCCESS);


        result.response = JsonResponsePacketSerializer::serializeGetRoomsResponse(getRoomsResponse);

        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;

        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);
    }
    
    return result;
}

RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& request) const
{
    RequestResult result;
    try
    {
        GetPlayersInRoomRequest getPlayersInRoomRequest = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(request.buffer);
        GetPlayersInRoomResponse getPlayersInRoomResponse;

        auto room = m_handlerFactory.getRoomManager().getRoom(getPlayersInRoomRequest.roomId);
        if (room.has_value())
        {
            getPlayersInRoomResponse.players = room->get().getAllUser();
        }
        else
        {
            getPlayersInRoomResponse.players = {};
        }
        
        result.response = JsonResponsePacketSerializer::serializeGetPlayersInRoomResponse(getPlayersInRoomResponse);

        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;

        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);
    }

    return result;
}

RequestResult MenuRequestHandler::getPersonalStats(const RequestInfo& request) const
{
    RequestResult result;
   
    try
    {
        GetPersonalStatsResponse getPersonalStatsResponse;

        getPersonalStatsResponse.statistics = m_handlerFactory.getStatisticsManager().getUserStatistics(m_user.getUserName());
        if (m_handlerFactory.getLoginManager().doesUserExist(m_user.getUserName()))
        {
            getPersonalStatsResponse.status = (int)Menu_Handler_Statuses::GET_STATISTICS_SUCCESS;
        }
        else
        {
            getPersonalStatsResponse.status = (int)Menu_Handler_Statuses::USER_DOESNT_EXIST;
        }

        result.response = JsonResponsePacketSerializer::serializeGetPersonalStatsResponse(getPersonalStatsResponse);

        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;

        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);
    }

   
    return result;
}

RequestResult MenuRequestHandler::getHighScore(const RequestInfo& request) const
{
    RequestResult result;
    try
    {
        GetHighScoreResponse getHighScoreResponse;

        getHighScoreResponse.statistics = m_handlerFactory.getStatisticsManager().getHighScore();
        getHighScoreResponse.status = (int)Menu_Handler_Statuses::GET_HIGH_SCORES_SUCCESS;

        result.response = JsonResponsePacketSerializer::serializeGetHighScoreResponse(getHighScoreResponse);

        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;

        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);
    }

    return result;
}

RequestResult MenuRequestHandler::joinRoom(const RequestInfo& request) const
{
    RequestResult result;
    try
    {
        JoinRoomRequest joinRoomRequest = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(request.buffer);
        JoinRoomResponse joinRoomResponse;

        auto room = m_handlerFactory.getRoomManager().getRoom(joinRoomRequest.roomId);
        if (room.has_value())
        {
            joinRoomResponse.status = static_cast<int>(room->get().addUser(m_user));
            result.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, room.value());
        }
        else
        {
            joinRoomResponse.status = static_cast<int>(Menu_Handler_Statuses::ROOM_NOT_FOUND);
            result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
        }

        result.response = JsonResponsePacketSerializer::serializeJoinRoomResponse(joinRoomResponse);

    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;

        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);
    }

    return result;
}

RequestResult MenuRequestHandler::createRoom(const RequestInfo& request) const
{
    RequestResult result;
    try
    {
        CreateRoomRequest createRoomRequest = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(request.buffer);
        CreateRoomResponse createRoomResponse;

        RoomData roomData;
        roomData.id = m_handlerFactory.getRoomManager().getLastRoomId() + 1;
        roomData.name = createRoomRequest.roomName;
        roomData.maxPlayers = createRoomRequest.maxUsers;
        roomData.numOfQuestionsInGame = createRoomRequest.questionCount;
        roomData.timePerQuestion = createRoomRequest.answerTimeout;
        roomData.status = RoomState::DIDNT_START;

        createRoomResponse.status = (int)m_handlerFactory.getRoomManager().createRoom(m_user, roomData);
 
        result.response = JsonResponsePacketSerializer::serializeCreateRoomResponse(createRoomResponse);

        auto room = m_handlerFactory.getRoomManager().getRoom(roomData.id);
        if (room && createRoomResponse.status == (int)Menu_Handler_Statuses::CREATE_ROOM_SUCCESS)
        {
            result.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_user, room->get());
        }
        else
        {
            result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
        }

    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;

        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);
    }

    return result;
}

void MenuRequestHandler::logOutUserIfCrash()
{
    m_handlerFactory.getLoginManager().logout(m_user.getUserName());
}
