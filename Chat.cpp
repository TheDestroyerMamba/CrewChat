#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
//===================================================================================================
#include <iostream>

using namespace std;
//===================================================================================================
#include "Chat.hpp"
//===================================================================================================

void closeSocket(int sock)
{
	close(sock);
	return;
}

int createSocket(char* destination, int port)
{
	struct sockaddr_in temp; 
	struct hostent *h;
	int sock;
	int errore;

	//Tipo di indirizzo
	temp.sin_family = AF_INET;
	temp.sin_port = htons(port);
	h = gethostbyname(destination);
	if(h == 0) {
		printf("Gethostbyname fallito\n");
		exit(1);
	}
	bcopy(h->h_addr, &temp.sin_addr, h->h_length);
	//Creazione socket. 
	sock = socket(AF_INET, SOCK_STREAM, 0);
	//Connessione del socket. Esaminare errore per compiere azioni
	//opportune in caso di errore.
	errore = connect(sock, (struct sockaddr*) &temp, sizeof(temp));
	return sock;
}

void sendMessage(int sock, char* message)
{
	cout << message << endl;
	//Si puo' notare il semplice utilizzo di write: 
	//write(socket, messaggio, lunghezza messaggio)
	if (write(sock, message, strlen(message)) < 0)
	{
		cout << "Impossibile mandare il messaggio." << endl;
		closeSocket(sock);
		exit(1);
	}  
	cout << "Messaggio spedito con successo." << endl;
	return;
}
