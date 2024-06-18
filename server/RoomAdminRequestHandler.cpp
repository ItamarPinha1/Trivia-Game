#include "RoomAdminRequestHandler.h"

#include "JsonResponsePacketSerializer.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(const LoggedUser& user, RoomManager& roomManager, const Room& room, RequestHandlerFactory& handleFactory) : m_user(user), m_handlerFactory(handleFactory), m_roomManager(roomManager), m_room(room)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
	return request.id == (int)MessageType::MT_CLIENT_GET_ROOM_STATE || request.id == (int)MessageType::MT_CLIENT_CLOSE_ROOM || request.id == (int)MessageType::MT_CLIENT_START_GAME;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& request)
{
    RequestResult result;
    switch (request.id)
    {
    case MT_CLIENT_GET_ROOM_STATE:
    {
        result = getRoomState(request);
        break;
    }
    case MT_CLIENT_CLOSE_ROOM:
    {
        result = closeRoom(request);
        break;
    }
    case MT_CLIENT_START_GAME:
    {
        result = startGame(request);
        break;
    }
    default:
    {
        ErrorResponse errorResponse;
        errorResponse.message = "Invalid request ID";

        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);
        result.newHandler = m_handlerFactory.createLoginRequestHandler();
        break;
    }
    }

    return result;
}

void RoomAdminRequestHandler::logOutUserIfCrash()
{
    m_room.removeUser(m_user);
    m_handlerFactory.createMenuRequestHandler(m_user)->logOutUserIfCrash();
    m_room.setRoomState(RoomState::ROOM_CLOSE);
    m_roomManager.deleteRoom(m_room.getRoomData().id);
}



RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo& request)
{
    RequestResult result;

    try
    {

        CloseRoomResponse closeRoomResponse;
        if (m_roomManager.deleteRoom(m_room.getRoomData().id) == Menu_Handler_Statuses::ROOM_DELETED_SUCCESSFULLY)
        {
            m_room.setRoomState(RoomState::ROOM_CLOSE);
            closeRoomResponse.status = (int)In_Room_Statueses::CLOSE_ROOM_SUCCES;
        }

        result.response = JsonResponsePacketSerializer::serializeCloseRoomResponse(closeRoomResponse);

        if (closeRoomResponse.status == (int)In_Room_Statueses::CLOSE_ROOM_SUCCES)
        {
            result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
        }
        else
        {
            result.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_user, m_room);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;
        errorResponse.message = "Room doesnt Exist";

        result.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_user, m_room);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);

    }

    return result;
}

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo& request) 
{
    RequestResult result;

    try
    {

        StartGameResponse startGameResponse;
        auto room = m_roomManager.getRoom(m_room.getRoomData().id);

        if (room)
        {
            startGameResponse.status = (int)In_Room_Statueses::START_GAME_SUCCES;
            room->get().setRoomState(RoomState::IN_GAME);

            Game& game = m_handlerFactory.getGameManager().createGame(room->get(), room->get().getRoomData().id);

            result.newHandler = m_handlerFactory.createGameRequestHandler(game, m_user);

        }
        else
        {
            startGameResponse.status = (int)In_Room_Statueses::START_GAME_FAIL;
            result.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_user, m_room);
        }

        result.response = JsonResponsePacketSerializer::serializeStartGameResponse(startGameResponse);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;
        errorResponse.message = "Room doesnt Exist";

        result.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_user, m_room);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);

    }

    return result;
}


RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo& request) const
{
    RequestResult result;

    try
    {
        GetRoomStateResponse getRoomStateResponse;

        auto room = m_roomManager.getRoom(m_room.getRoomData().id);

        if (room)
        {
            getRoomStateResponse.roomState = room->get().getRoomState();
            getRoomStateResponse.players = room->get().getAllUser();
            getRoomStateResponse.answerTimeout = m_room.getRoomData().timePerQuestion;
            getRoomStateResponse.questionCount = m_room.getRoomData().numOfQuestionsInGame;
            getRoomStateResponse.status = (int)In_Room_Statueses::GET_ROOM_STATE_SUCCES;

        }
        else
        {
            getRoomStateResponse.roomState = RoomState::DOESNT_EXIST;
            getRoomStateResponse.players = {};
            getRoomStateResponse.answerTimeout = 0;
            getRoomStateResponse.questionCount = 0;
            getRoomStateResponse.status = (int)In_Room_Statueses::GET_ROOM_STATE_FAIL;
        }

        result.response = JsonResponsePacketSerializer::serializeGetRoomStateResponse(getRoomStateResponse);

        if (getRoomStateResponse.status == (int)In_Room_Statueses::GET_ROOM_STATE_SUCCES && getRoomStateResponse.roomState != RoomState::IN_GAME)
        {
            result.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_user, m_room);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;
        errorResponse.message = "Room doesnt Exist";

        result.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_user, m_room);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);

    }

    return result;
}
