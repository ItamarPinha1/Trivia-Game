#include "Server.h"

int main()
{
	WSAInitializer wsa_init;
	Server server;
	server.run();
}