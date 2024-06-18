#pragma once

#include "Helper.h"

#include <ctime>
#include <vector>



class IRequestHandler;


struct RequestInfo
{
	unsigned char id;
	std::time_t receivalTime;
	std::vector<char> buffer;

};


struct RequestResult
{
	std::vector<char> response;
	std::unique_ptr<IRequestHandler> newHandler;
};

class IRequestHandler
{
public:
	virtual bool isRequestRelevant(const RequestInfo& request) const = 0;
	virtual RequestResult handleRequest(const RequestInfo& request) = 0;
	virtual void logOutUserIfCrash() = 0;
};

