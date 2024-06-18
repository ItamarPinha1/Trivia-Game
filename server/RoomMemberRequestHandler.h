#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

class RoomMemberRequestHandler : public IRequestHandler
{
public:
	RoomMemberRequestHandler(const LoggedUser& user, RoomManager& roomManager , const Room& room, RequestHandlerFactory& handleFactory);

	bool isRequestRelevant(const RequestInfo& request) const override;
	RequestResult handleRequest(const RequestInfo& request) override;
	void logOutUserIfCrash() override;

private:
	LoggedUser m_user;
	Room m_room;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult leaveRoom(const RequestInfo& request);
	RequestResult getRoomState(const RequestInfo& request) const;
};