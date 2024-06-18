#pragma once
#include "IRequestHandler.h"

#include "RequestHandlerFactory.h"


class MenuRequestHandler : public IRequestHandler
{
private:
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult signout(const RequestInfo& request) const;
	RequestResult getRooms(const RequestInfo& request) const;
	RequestResult getPlayersInRoom(const RequestInfo& request) const;
	RequestResult getPersonalStats(const RequestInfo& request) const;
	RequestResult getHighScore(const RequestInfo& request) const;
	RequestResult joinRoom(const RequestInfo& request) const;
	RequestResult createRoom(const RequestInfo& request) const;


public:
	MenuRequestHandler(LoggedUser user, RequestHandlerFactory& handleFactory);

	bool isRequestRelevant(const RequestInfo& request) const override;
	RequestResult handleRequest(const RequestInfo& request) override;
	void logOutUserIfCrash() override;
	
};

