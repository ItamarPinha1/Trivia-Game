#pragma once
#include "IRequestHandler.h"
#include "GameManager.h"
#include "RequestHandlerFactory.h"


enum Answers_Numbers : int
{
	NUMBER_OF_QUESTIONS = 4,
};


class GameRequestHandler : public IRequestHandler
{

public:
	GameRequestHandler(Game& game, const LoggedUser& user, GameManager& gameManager, RequestHandlerFactory& factory);

	bool isRequestRelevant(const RequestInfo& request) const override;
	RequestResult handleRequest(const RequestInfo& request) override;
	void logOutUserIfCrash() override;

private:

	Game& m_game;
	LoggedUser m_user;
	GameManager& m_gameManager;
	RequestHandlerFactory& m_handleFactory;

	RequestResult getQuestion(const RequestInfo& request) const;
	RequestResult submitAnswer(const RequestInfo& request) const;
	RequestResult getGameResults(const RequestInfo& request) const;
	RequestResult leaveGame(const RequestInfo& request) const;


};

