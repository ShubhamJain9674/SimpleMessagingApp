#include "App.h"



void App::init()
{
	ConnectedSSID = GetConnectedWifiSSID();
	IP = getLocalIP();

}


void App::StartApp()
{
	int choice = 1;

	std::cout << "connected SSID : " << ConnectedSSID << std::endl;

	std::cout << "Enter 1 to start a server " << std::endl;
	std::cout << "Enter 2 to join a server " << std::endl;
	std::cout << "Enter 0 to exit : " << std::endl;


	while (RUN) {

		std::cout << "Enter your choice : ";
		std::cin >> choice;
		switch (choice) {
			case 0:
				std::cout << "Closing the program !" << std::endl;
				RUN = 0;
				break;
			case 1:
			{




				std::cout << "starting server!" << std::endl;
				myServer.StartServer();  // Initialize and bind the server
				myServer.Listen();       // Start listening for connections

				std::thread ServerAcceptThread(&Server::Accept,&myServer);
				Sleep(5000);
				std::thread ServerSendMessageThread(&Server::ServerSendMessage,&myServer);
				ServerAcceptThread.join();
				ServerSendMessageThread.join();
				myServer.Cleanup();
				break;
			}
			case 2:
			{
				std::cout << "Enter IP address: ";
				std::cin >> IPtoConnect;
				//ConnectServer newServer;
				if (isValidIP(IPtoConnect)) {

					std::thread ClientConnectThread(&ConnectServer::Connect,&newServer,IPtoConnect);
					Sleep(5000);
					std::thread RecieveMessageFromServer(&ConnectServer::RecieveMessage,&newServer);

					ClientConnectThread.join();
					RecieveMessageFromServer.join();
					newServer.Cleanup();
				}
				else {
					std::cout << "The IP address is invalid." << std::endl;
				}
				break;
			

			}
			default:
				std::cout << "invalid input ! " << std::endl;
		}

	}

	std::cout << "*******************************************" << std::endl;


}


