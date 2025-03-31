
#include "App.h"



int main() {

	App myApp;
	myApp.RUN = 1;
	myApp.init();
	myApp.StartApp();
	
	return 0;
}

//bugs in multithreading check
//i am using accept socket to recieve message but that is not present in 
// client device i have to bring client code under server class and use clientsocket instead