#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>
#include <wlanapi.h>


#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

std::string getLocalIP();

std::string GetConnectedWifiSSID();


class Server {

private:
	WSADATA wsaData;
	int wsaErr;

	WORD wVersionRequested = MAKEWORD(2, 2);

	std::string localIP;
	SOCKET serverSocket;

public:

	SOCKET acceptSocket;
	sockaddr_in ClientSocket;
	int clientSocketSize;
	char ClientIP[INET_ADDRSTRLEN];
	int serverPort = 55555;

	void StartServer();
	void Cleanup();
	void Listen();
	bool Accept();
	void CloseServer();
	void ServerSendMessage();

};



