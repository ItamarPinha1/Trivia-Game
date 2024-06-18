#pragma once

#include "Game.h"
#include "Room.h"
#include <optional>
#include "JsonResponsePacketSerializer.h" // need it for PlayerResults struct

class GameManager
{
public:
	GameManager(const std::weak_ptr<IDatabase>& db);

	Game& createGame(const Room& room, const int gameId);
	void deleteGame(const int gameId);

	std::optional<std::reference_wrapper<Game>> getGame(const int gameId);

	std::vector<PlayerResults> getGameResults(const int gameId);
	

private:
	std::weak_ptr<IDatabase> m_database;
	std::vector<Game> m_games;

};

