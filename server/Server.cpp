#include "Server.h"



Server::Server() : m_database(std::make_shared<SqliteDatabase>()), m_handleFactory(m_database), m_communicator(m_handleFactory)
{
}

void Server::run()
{
	try
	{

		TRACE("Starting...");
		// NOTICE at the end of this block the WSA will be closed 
		m_communicator.startHandleRequests();
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception was thrown in function: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown exception in main !" << std::endl;
	}
}
