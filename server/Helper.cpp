#include "Helper.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using std::string;

// recieves the type code of the message from socket (1 bytes)
// and returns the code. if no message found in the socket returns 0 (which means the client disconnected)
std::vector<char> Helper::getMessageTypeCodeByte(const SOCKET sc)
{
	std::vector<char> msg = getPartFromSocket(sc, 1, 0);

	return msg;
}

// recieve data from socket according byteSize
// returns the data as int
int Helper::getLenPartFromSocket(const SOCKET sc, const int bytesNum)
{
	std::vector<char> dataSizeByte = getPartFromSocket(sc, bytesNum, 0);
	int len = buffToInteger(dataSizeByte, bytesNum);

	return len;
}

// recieve data from socket according byteSize
// returns the data as string
std::vector<char> Helper::getJsonPartFromSocket(const SOCKET sc, const int bytesNum)
{
	return getPartFromSocket(sc, bytesNum, 0);
}

// return string after padding zeros if necessary
string Helper::getPaddedNumber(const int num, const int digits)
{
	std::ostringstream ostr;
	ostr << std::setw(digits) << std::setfill('0') << num;
	return ostr.str();

}

int Helper::buffToInteger(std::vector<char> buffer, const int numBytes)
{
	int result = 0;

	for (int i = 0; i < buffer.size(); i++)
	{
		result <<= 8;
		result |= (buffer[i] & 0xFF);
	}

	return result;
}

// recieve data from socket according byteSize
// this is private function
std::vector<char> Helper::getPartFromSocket(const SOCKET sc, const int bytesNum)
{
	return getPartFromSocket(sc, bytesNum, 0);
}

// send data to socket
// this is private function
void Helper::sendData(const SOCKET sc, const std::vector<char> message)
{
	const char* data = message.data();

	if (send(sc, data, message.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}
}

std::vector<char> Helper::getPartFromSocket(const SOCKET sc, const int bytesNum, const int flags)
{
	if (bytesNum == 0)
	{
		return {};
	}

	std::vector<char> data(bytesNum);
	int res = recv(sc, data.data(), bytesNum, flags);
	if (res == INVALID_SOCKET)
	{
		std::string s = "Error while recieving from socket: ";
		s += std::to_string(sc);
		throw std::exception(s.c_str());
	}
	data.resize(res);
	return data;
}
