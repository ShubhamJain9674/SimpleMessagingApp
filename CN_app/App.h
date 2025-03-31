#pragma once

#include "Server.h"
#include "ConnectServer.h"
#include <thread>


class App {

private:
	
	//
public:
	int RUN{ 1 };
	std::string IP;
	std::string ConnectedSSID;
	std::string IPtoConnect;
	Server myServer;
	ConnectServer newServer;

	void init();
	void StartApp();


};