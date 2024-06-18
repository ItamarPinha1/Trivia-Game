#pragma once
#include <winsock2.h> 
#include <Windows.h>
#include <exception>
#include <iostream>
#include <string>
#include <numeric>
#include <map>
#include <queue>
#include <mutex>
#include "Helper.h"
#include "RequestHandlerFactory.h"
#include "SqliteDataBase.h"

#include <memory>


class Communicator
{
public:
	Communicator(RequestHandlerFactory& handleFactory);
	~Communicator();
	void startHandleRequests();

private:
	SOCKET m_serverSocket;
	std::map<SOCKET, std::unique_ptr<IRequestHandler>> m_clients;

	void bindAndListen();
	void handleNewClient(const SOCKET client_socket);

	RequestInfo build_receive_message(const SOCKET client_socket, const int msg_code);
	void acceptClient();

	mutable std::shared_mutex clients_mtx;
	RequestHandlerFactory& m_handleFactory;


};

