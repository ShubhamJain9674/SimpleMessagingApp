#include "ConnectServer.h"


bool isValidIP(const std::string& ip) {
	std::stringstream ss(ip);
	std::string segment;
	int value;
	int segmentCount = 0;

	while (std::getline(ss, segment, '.')) {
		// Check if the segment is an integer and in the range [0, 255]
		try {
			value = std::stoi(segment);
			if (value < 0 || value > 255) return false;
			segmentCount++;
		}
		catch (const std::invalid_argument&) {
			return false;
		}
		catch (const std::out_of_range&) {
			return false;
		}
	}

	return segmentCount == 4; // Should have exactly 4 segments
}


void ConnectServer::Connect(std::string IP) {
	WSADATA wsaData;
	int wsaErr;

	WORD wVersionRequested = MAKEWORD(2, 2);

	wsaErr = WSAStartup(wVersionRequested, &wsaData);

	if (wsaErr != 0) {
		std::cout << "The Winsock dll not found ! " << std::endl;
		return;
	}
	else {
		std::cout << "The Winsock dll found ! " << std::endl;
		std::cout << "The status : " << wsaData.szSystemStatus << std::endl;
	}

	mySocket = INVALID_SOCKET;
	mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (mySocket == INVALID_SOCKET) {

		std::cout << "Error at socket():" << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}
	else {
		std::cout << "socket() is ok!" << std::endl;
	}

	sockaddr_in ClientService;
	ClientService.sin_family = AF_INET;

	std::wstring wideIP(IP.begin(), IP.end());

	if (InetPtonW(AF_INET, wideIP.c_str(), &ClientService.sin_addr) != 1) {
		std::cout << "InetPton() failed: " << WSAGetLastError() << std::endl;
		closesocket(mySocket);
		WSACleanup();
		return;
	}
	ClientService.sin_port = htons(55555);

	if (connect(mySocket, (SOCKADDR*)&ClientService, sizeof(ClientService)) == SOCKET_ERROR) {
		std::cout << "Client connet() - Failed to connect" << std::endl;
		closesocket(mySocket);
		WSACleanup();
		return;
	}
	else {
		std::cout << "Client Connect(): is OK." << std::endl;
		std::cout << "Client: Can start sending and Recieving data..." << std::endl;
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
		int byteCount = send(mySocket, buffer, 200, 0);
		if (byteCount == SOCKET_ERROR) {
			printf("Server send error %ld.\n", WSAGetLastError());

			closesocket(mySocket);
			WSACleanup();
			return;
		}
		else {
			printf("Server sent %ld bytes \n", byteCount);
		}



	}

	closesocket(mySocket);
	WSACleanup();
}

void ConnectServer::RecieveMessage() {

	char receiveBuffer[200] = "";

	while (strcmp(receiveBuffer, "over") != 0) {

		memset(receiveBuffer, 0, sizeof(receiveBuffer));

		int byteCount = recv(mySocket, receiveBuffer, 200, 0);

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
}

void ConnectServer::Cleanup()
{
	if (mySocket != INVALID_SOCKET) {
		closesocket(mySocket);
	}
	WSACleanup();

}


