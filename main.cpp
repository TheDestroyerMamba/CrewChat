#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>

#include "communication.h"

using namespace std;

int main(void) {
	
	string dest_IP;

	system("clear");
	cout << "Inserire l'IP dell'altro utente: ";
	cin >> dest_IP;
	system("clear");

	int counter = 0;
	pid_t pid = fork();
	if (pid == 0)
	{
		// child process
		listenForMessages();
	}
	else if (pid > 0)
	{

		int Socket;

		//Creo e connetto il socket
		Socket = createClientSocket(dest_IP.c_str, _PORT);

		bool quit = false;
		string message;

		while (!quit) {
			cout << ">>> " << endl;
			cin >> message;

			//Spedisco il messaggio voluto
			if (strcmp(message.c_str, "\\quit") == 0))
				sendMessage(Socket, "\\quit");
			else
				sendMessage(Socket, "Un messaggio");

			//Chiudo il socket.
			closeSocket(Socket);
		}
	}
	else
	{
		// fork failed
		printf("fork() failed!\n");
		return 1;
	}

	system("clear");
	cout << "Grazie per aver utilizzato CrewChat!" << endl;
	
	return 1;
};
