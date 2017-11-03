#pragma once

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

void closeSocket(int sock);
int createServerSocket(int port);
int createClientSocket(char* destination, int port);
void sendMessage(int sock, char* message);

bool listenForMessages(void);


#define _PORT 6996


#endif