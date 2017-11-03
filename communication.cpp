#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
//===================================================================================================
#include <iostream>

using namespace std;
//===================================================================================================
#include "communication.h"
//===================================================================================================

void closeSocket(int sock)
{
	close(sock);
	return;
}

int createClientSocket(char* destination, int port)
{
	struct sockaddr_in temp;
	struct hostent *h;
	int sock;
	int errore;

	//Tipo di indirizzo
	temp.sin_family = AF_INET;
	temp.sin_port = htons(port);
	h = gethostbyname(destination);
	if (h == 0) {
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

int createServerSocket(int port) {
	int sock, errore;
	struct sockaddr_in temp;

	//Creazione socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	//Tipo di indirizzo
	temp.sin_family = AF_INET;
	temp.sin_addr.s_addr = INADDR_ANY;
	temp.sin_port = htons(port);

	//Il socket deve essere non bloccante
	errore = fcntl(sock, F_SETFL, O_NONBLOCK);

	//Bind del socket
	errore = bind(sock, (struct sockaddr*) &temp, sizeof(temp));
	//Per esempio, facciamo accettare fino a 7 richieste di servizio
	//contemporanee (che finiranno nella coda delle connessioni).
	errore = listen(sock, 7);

	return sock;
}

void sendMessage(int sock, char* message)
{
	cout << "\t" << message << endl;
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

bool listenForMessages(void) {
	char buffer[512];
	int DescrittoreSocket, NuovoSocket;
	int exitCond = 0;
	int Quanti;

	DescrittoreSocket = createServerSocket(_PORT);
	cout << "Aspettando un messaggio..." << endl;
	while (!exitCond)
	{
		//Test sul socket: accept non blocca, ma il ciclo while continua
		//l'esecuzione fino all'arrivo di una connessione.
		if ((NuovoSocket = accept(DescrittoreSocket, 0, 0)) != -1)
		{
			//Lettura dei dati dal socket (messaggio ricevuto)
			if ((Quanti = read(NuovoSocket, buffer, sizeof(buffer)))<0)
			{
				printf("Impossibile leggere il messaggio.\n");
				close(NuovoSocket);
			}
			else
			{
				//Aggiusto la lunghezza...
				buffer[Quanti] = 0;
				//Elaborazione dati ricevuti
				if (strcmp(buffer, "\\quit") == 0)
					exitCond = 1;
				else cout << "Utente: " << buffer << endl;
			}
			//Chiusura del socket temporaneo
			closeSocket(NuovoSocket);
		}
	}
	//Chiusura del socket
	closeSocket(DescrittoreSocket);
	printf("Utente: Terminato.\n");
}
