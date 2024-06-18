#pragma once

#include "LoginManager.h"
#include "IDatabase.h"
#include "LoginRequestHandler.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "GameManager.h"

#include <memory>

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(std::weak_ptr<IDatabase> database);

	std::unique_ptr<IRequestHandler> createLoginRequestHandler();
	std::unique_ptr<IRequestHandler> createMenuRequestHandler(const LoggedUser& user);

	std::unique_ptr<IRequestHandler> createRoomMemberRequestHandler(const LoggedUser& user, const Room& room);
	std::unique_ptr<IRequestHandler> createRoomAdminRequestHandler(const LoggedUser& user, const Room& room);

	std::unique_ptr<IRequestHandler> createGameRequestHandler(Game& game, const LoggedUser& user);

	LoginManager& getLoginManager();
	RoomManager& getRoomManager();
	StatisticsManager& getStatisticsManager();
	GameManager& getGameManager();

	std::weak_ptr<IDatabase> getDB() const;

private:
	LoginManager m_loginManager;
	RoomManager m_roomManger;
	StatisticsManager m_statisticsManager;
	GameManager m_gameManager;

	std::weak_ptr<IDatabase> m_database;

};

