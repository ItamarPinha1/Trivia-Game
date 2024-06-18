#include "RoomMemberRequestHandler.h"

#include "JsonResponsePacketSerializer.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(const LoggedUser& user, RoomManager& roomManager, const Room& room, RequestHandlerFactory& handleFactory) : m_user(user), m_handlerFactory(handleFactory), m_roomManager(roomManager), m_room(room)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
    return request.id == (int)MessageType::MT_CLIENT_GET_ROOM_STATE || request.id == (int)MessageType::MT_CLIENT_LEAVE_ROOM;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& request)
{
    RequestResult result;
    switch (request.id)
    {
    case MT_CLIENT_GET_ROOM_STATE:
    {
        result = getRoomState(request);
        break;
    }
    case MT_CLIENT_LEAVE_ROOM:
    {
        result = leaveRoom(request);
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

void RoomMemberRequestHandler::logOutUserIfCrash()
{
    m_room.removeUser(m_user);
    m_handlerFactory.createMenuRequestHandler(m_user)->logOutUserIfCrash();
}


RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo& request)
{
    RequestResult result;

    try
    {

        LeaveRoomResponse leaveRoomResponse;

        auto room = m_roomManager.getRoom(m_room.getRoomData().id);

        if (room)
        {
            leaveRoomResponse.status = (int)room->get().removeUser(m_user);
        }
        else
        {
            leaveRoomResponse.status = (int)In_Room_Statueses::LEAVE_ROOM_FAIL;
        }

        result.response = JsonResponsePacketSerializer::serializeLeaveRoomResponse(leaveRoomResponse);

        if (leaveRoomResponse.status == (int)In_Room_Statueses::LEAVE_ROOM_SUCCES)
        {
            result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
        }
        else
        {
            result.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, m_room);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;
        errorResponse.message = "Room doesnt Exist";

        result.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, m_room);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);

    }

    return result;
}

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo& request) const
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

            if (getRoomStateResponse.roomState == RoomState::IN_GAME)
            {
                auto game = m_handlerFactory.getGameManager().getGame(room->get().getRoomData().id);

                if (game)
                {
                    result.newHandler = m_handlerFactory.createGameRequestHandler(game->get(), m_user);
                }
                else
                {
                    result.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, m_room);
                }
            }
            else
            {
                result.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, m_room);
            }
        }
        else
        {
            getRoomStateResponse.roomState = RoomState::DOESNT_EXIST;
            getRoomStateResponse.players = {};
            getRoomStateResponse.answerTimeout = 0;
            getRoomStateResponse.questionCount = 0;
            getRoomStateResponse.status = (int)In_Room_Statueses::GET_ROOM_STATE_FAIL;
            result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
        }

        result.response = JsonResponsePacketSerializer::serializeGetRoomStateResponse(getRoomStateResponse);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;
        errorResponse.message = "Room doesnt Exist";

        result.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, m_room);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);

    }

    return result;
}
