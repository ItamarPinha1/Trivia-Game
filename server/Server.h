#pragma once
#pragma comment (lib, "ws2_32.lib")
#include "WSAInitializer.h"
#include <iostream>
#include <fstream>
#include "Communicator.h"

#include "IDatabase.h"
#include "RequestHandlerFactory.h"

#include <memory>


class Server
{
private:

	std::shared_ptr<IDatabase> m_database;
	RequestHandlerFactory m_handleFactory;
	Communicator m_communicator;

public:
	Server();
	void run();
};

