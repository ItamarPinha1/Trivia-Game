#include "RequestHandlerFactory.h"

#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "GameRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory(std::weak_ptr<IDatabase> database) : 
	m_database(database), m_loginManager(database), m_statisticsManager(database), m_gameManager(database)
{
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createLoginRequestHandler()
{
	return std::make_unique<LoginRequestHandler>(*this);
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user)
{
	return std::make_unique<MenuRequestHandler>(user ,*this);
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser& user, const Room& room)
{
	return std::make_unique<RoomMemberRequestHandler>(user, m_roomManger, room, *this);
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser& user, const Room& room)
{
	return std::make_unique<RoomAdminRequestHandler>(user, m_roomManger, room, *this);
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createGameRequestHandler(Game& game, const LoggedUser& user)
{
	return std::make_unique<GameRequestHandler>(game, user, m_gameManager, *this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return this->m_loginManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
	return this->m_roomManger;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
	return this->m_statisticsManager;
}

GameManager& RequestHandlerFactory::getGameManager()
{
	return this->m_gameManager;
}

std::weak_ptr<IDatabase> RequestHandlerFactory::getDB() const
{
	return m_database;
}
