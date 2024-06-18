#include "Communicator.h"

static const unsigned short PORT = 23455;
static const unsigned int IFACE = 0;

using std::string;
using std::mutex;
using std::unique_lock;
using std::vector;

Communicator::Communicator(RequestHandlerFactory& handleFactory) : m_handleFactory(handleFactory)
{
	// notice that we step out to the global namespace
	// for the resolution of the function socket
	m_serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");



}

Communicator::~Communicator()
{
	TRACE(__FUNCTION__ " closing accepting socket");
	// why is this try necessarily ?
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		::closesocket(m_serverSocket);
	}
	catch (...) {}
}

void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = IFACE;
	// again stepping out to the global namespace
	if (::bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	TRACE("binded");

	if (::listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	TRACE("listening...");
}

RequestInfo Communicator::build_receive_message(const SOCKET client_socket, const int msg_code)
{
	RequestInfo info;

	info.id = msg_code;
	info.receivalTime = std::time(nullptr);// Set the receival time to the current time

	int dataSize = Helper::getLenPartFromSocket(client_socket, DATA_SIZE_BYTE_COUNT);// get len of data

	std::vector<char> data = Helper::getJsonPartFromSocket(client_socket, dataSize); // get the data as string

	info.buffer.assign(data.begin(), data.end());// convert the string to vector of BYTES

	return info;

}


void Communicator::acceptClient()
{
	SOCKET client_socket = accept(m_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	TRACE("Client accepted !");
	// create new thread for client	and detach from it
	std::thread tr(&Communicator::handleNewClient, this, client_socket);

	std::unique_lock lock(clients_mtx);
	m_clients[client_socket] = m_handleFactory.createLoginRequestHandler();
	tr.detach();
}

void Communicator::handleNewClient(const SOCKET client_socket)
{
	RequestInfo currRcvMsg;
	RequestResult currRsltMsg;
	try {
		do 
		{
			// get the first message
			std::vector<char> idByte = Helper::getMessageTypeCodeByte(client_socket);
			int id = Helper::buffToInteger(idByte, 1);
			currRcvMsg = build_receive_message(client_socket, id);


			currRsltMsg = m_clients[client_socket]->handleRequest(currRcvMsg);

			Helper::sendData(client_socket, currRsltMsg.response);

			std::shared_lock lock(clients_mtx);
			m_clients[client_socket] = std::move(currRsltMsg.newHandler);
		}
		while (m_clients[client_socket] != nullptr);

	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred in function handleNewClient: " << e.what() << std::endl;

		m_clients[client_socket]->logOutUserIfCrash();
	}

	std::unique_lock lock(clients_mtx);
	m_clients.erase(client_socket);

	closesocket(client_socket);
}

void Communicator::startHandleRequests()
{
	bindAndListen();


	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		TRACE("accepting client...");
		acceptClient();
	}
}
