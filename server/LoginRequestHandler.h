#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{

public:
	LoginRequestHandler(RequestHandlerFactory& handleFactory);

	bool isRequestRelevant(const RequestInfo& request) const override;
	RequestResult handleRequest(const RequestInfo& request) override;
	void logOutUserIfCrash() override; // dont need to implement


private:

	RequestHandlerFactory& m_handleFactory;

	RequestResult login(const RequestInfo& request) const;
	RequestResult signup(const RequestInfo& request) const;
};

