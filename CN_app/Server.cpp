#include "Server.h"
#include <cstring>

std::string getLocalIP() {
	// Initialize Windows sockets
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Get adapter info
	IP_ADAPTER_INFO adapterInfo[16];
	DWORD bufferSize = sizeof(adapterInfo);

	// Retrieve the list of adapters
	DWORD dwStatus = GetAdaptersInfo(adapterInfo, &bufferSize);
	if (dwStatus != ERROR_SUCCESS) {
		std::cerr << "Error getting adapter info!" << std::endl;
		return "";
	}

	// Loop through the adapters and display the IP address
	IP_ADAPTER_INFO* pAdapterInfo = adapterInfo;
	while (pAdapterInfo) {
		if (pAdapterInfo->IpAddressList.IpAddress.String[0] != '\0' &&
			(strcmp(pAdapterInfo->IpAddressList.IpAddress.String, "0.0.0.0") != 0)) {
			//std::cout << "Local IP Address: " << pAdapterInfo->IpAddressList.IpAddress.String << std::endl;
			std::string IP = pAdapterInfo->IpAddressList.IpAddress.String;
			return IP;
		}
		pAdapterInfo = pAdapterInfo->Next;
	}

	// Clean up
	WSACleanup();
	return "";
}


void Server::Cleanup() {

	if (serverSocket != INVALID_SOCKET) {
		closesocket(serverSocket);
	}
	WSACleanup();

}

void Server::Listen()
{
	if (listen(serverSocket, 1) == SOCKET_ERROR) {
		std::cout << "listen(): Error listening on socket" << WSAGetLastError() << std::endl;
	}
	else {
		std::cout << "listen(): is ok ,waiting for connections..." << std::endl;
	}
}

bool Server::Accept()
{
	clientSocketSize = sizeof(ClientSocket);
	acceptSocket = accept(serverSocket, (SOCKADDR*)&ClientSocket, &clientSocketSize);

	if (acceptSocket == INVALID_SOCKET) {
		std::cout << "accept failed:" << WSAGetLastError() << std::endl;
		return false;
	}

	inet_ntop(AF_INET, &(ClientSocket.sin_addr), ClientIP, INET_ADDRSTRLEN);
	std::cout << "Client IP Address : " << ClientIP << std::endl;


	char receiveBuffer[200] = "";

	while (strcmp(receiveBuffer, "over") != 0) {

		memset(receiveBuffer, 0, sizeof(receiveBuffer));

		int byteCount = recv(acceptSocket, receiveBuffer, 200, 0);

		if (byteCount < 0) {
			printf("Client error %ld.\n", WSAGetLastError());
			break;
		}
		else if (byteCount == 0) {
			std::cout << "Client Disconnected!" << std::endl;
			break;
		}
		else {
			receiveBuffer[byteCount - 1] = '\0';
			printf("Received data : %s \n", receiveBuffer);
		}
	}
	return true;
}

void Server::CloseServer()
{
	Cleanup();
}



void Server::StartServer() {

	/*WSADATA wsaData;
	int wsaErr;

	WORD wVersionRequested = MAKEWORD(2, 2);*/

	wsaErr = WSAStartup(wVersionRequested, &wsaData);

	if (wsaErr != 0) {
		std::cout << "The Winsock dll not found ! " << std::endl;
		return;
	}
	else {
		std::cout << "The Winsock dll found ! " << std::endl;
		std::cout << "The status : " << wsaData.szSystemStatus << std::endl;
	}

	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSocket == INVALID_SOCKET) {

		std::cout << "Error at socket Creation" << WSAGetLastError() << std::endl;
		Cleanup();
		return;
	}
	else {
		std::cout << "socket() is ok!" << std::endl;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;

	localIP = getLocalIP();

	if (localIP.empty()) {
		std::cout << "No valid local IP address found!" << std::endl;
		return;
	}

	// Convert std::string to std::wstring (wide string)
	std::wstring wideIP(localIP.begin(), localIP.end());



	if (InetPtonW(AF_INET, wideIP.c_str(), &service.sin_addr) != 1) {
		std::cout << "InetPton() failed: " << WSAGetLastError() << std::endl;
		Cleanup();
		return;
	}
	service.sin_port = htons(serverPort);

	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		std::cout << "bind() failed" << WSAGetLastError() << std::endl;
		Cleanup();
		return;
	}
	else {
		std::cout << "bind is ok!" << std::endl;
	}





	/*ClientIP[INET_ADDRSTRLEN];*/




	/*closesocket(serverSocket);
	WSACleanup();*/
}



std::string GetConnectedWifiSSID() {
	HANDLE hClient = NULL;
	DWORD dwMaxClient = 2;
	DWORD dwCurVersion = 0;
	DWORD dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		return "Failed to open WLAN handle";
	}

	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
	if (WlanEnumInterfaces(hClient, NULL, &pIfList) != ERROR_SUCCESS) {
		WlanCloseHandle(hClient, NULL);
		return "Failed to enumerate interfaces";
	}

	std::string ssid = "Not connected";

	for (int i = 0; i < (int)pIfList->dwNumberOfItems; i++) {
		PWLAN_INTERFACE_INFO pIfInfo = &pIfList->InterfaceInfo[i];

		PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
		DWORD connectInfoSize = sizeof(WLAN_CONNECTION_ATTRIBUTES);
		WLAN_OPCODE_VALUE_TYPE opCode;

		if (WlanQueryInterface(hClient, &pIfInfo->InterfaceGuid, wlan_intf_opcode_current_connection,
			NULL, &connectInfoSize, (PVOID*)&pConnectInfo, &opCode) == ERROR_SUCCESS) {
			ssid = std::string(reinterpret_cast<const char*>(pConnectInfo->wlanAssociationAttributes.dot11Ssid.ucSSID),
				pConnectInfo->wlanAssociationAttributes.dot11Ssid.uSSIDLength);
			WlanFreeMemory(pConnectInfo);
		}
	}

	WlanFreeMemory(pIfList);
	WlanCloseHandle(hClient, NULL);
	return ssid;
}


void Server::ServerSendMessage()
{
	if (acceptSocket == INVALID_SOCKET) {

		std::cout << "Error at socket():" << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}
	else {
		std::cout << "socket() is ok!" << std::endl;
	}


	char buffer[200];
	int RUN = 1;
	std::cin.ignore();
	while (RUN) {


		printf("Enter your message : ");
		std::cin.getline(buffer, 200);
		if (strcmp(buffer, "over") == 0) {
			RUN = 0;
		}
		int byteCount = send(acceptSocket, buffer, 200, 0);
		if (byteCount == SOCKET_ERROR) {
			printf("Server send error %ld.\n", WSAGetLastError());
			return;
		}
		else {
			printf("Server sent %ld bytes \n", byteCount);
		}

	}

}