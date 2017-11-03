#ifndef CHAT_HPP
#define CHAT_HPP

void closeSocket(int sock);
int createSocket(char* destination, int port);
void sendMessage(int sock, char* message);

#endif
