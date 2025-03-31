#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <cstring>
#include "Server.h"
#pragma comment(lib,"ws2_32.lib")


bool isValidIP(const std::string& ip);

//void RecieveMessage(Server ConnectedServer);

class ConnectServer {

private:
	//
	SOCKET mySocket{INVALID_SOCKET};

public:

	void Connect(std::string IP);
	void RecieveMessage();
	void Cleanup();
};