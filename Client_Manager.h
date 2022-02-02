#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma once

#include <winsock2.h>
#include <Windows.h>
#include <queue>
#include <string>
#include <stdlib.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib") 	// Use this library whilst linking - contains the Winsock2 implementation.

enum STATE { RESTING, SENDING, RECIEVING, LOOKINGFORCONNECTION, STARTCLIENT,HANDSHAKE,RECNONBLOCKING};


class Client_Manager
{
public:
	Client_Manager(std::queue<std::string>*, std::queue<std::string>*, std::queue<std::string>*);
	void Start_Client();								// Initialise Winsocket
	void Client_cleanup();								// Clean up Any Pointer/Sockets
	void Switch_state(STATE);							// Switch current state
	int Send_State();
	int Recive_State();
	int Handshake_state();
	void LookingforConnection_state();
	int Recive_NB_state();
	void Set_Socket_Blocking(int);
	void Clear_Memory_For_Buffer();
	void Close_Socket();
	STATE Get_state();									// Cheack state
	STATE state;
	SOCKET main_socket;
	SOCKADDR_IN  SockAddr;
	bool conected;

	// Client ID
	std::string Client_ID;

private:
	void Creat_Socket();
	WSADATA WsaDat;
	int server_address_size;

	

	//SOCKET main_socket;
	std::queue<std::string>* messages;
	std::queue<std::string>* msg_to_server;
	std::queue<std::string>* msg_from_server;
	//Buffer
	const int buffer_size = 1024;
	char buffer[1024];

};



