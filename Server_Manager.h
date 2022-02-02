#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <SFML/Graphics.hpp>

#pragma once

#include <winsock2.h>
#include <Windows.h>
#include <queue>
#include <string>
#include <deque>
#include <stdlib.h>
#include <iostream>
#include "Client.h"


#pragma comment(lib,"ws2_32.lib") 	// Use this library whilst linking - contains the Winsock2 implementation.

class Server_Manager
{
public:
	// Start Up
	Server_Manager(int,std::queue<std::string>*, std::deque<int>*,std::queue<int>*,int *,bool*);

	// Server States Functions
	void StartServer_state();
	int Recieve_state(Client*);
	int Send_state(Client*);
	int AcceptClient_state(Client*);
	int HandShake_state(Client*);
	void Listen_state();

	// Client Functions
	void Set_blocking(int,SOCKET *);
	void Add_client(int);
	void Clean_Cliens();
	bool cheack_client_name_exists(std::string);
	void Kick_Client(Client *);
	void Client_Quit(Client*);

	// Client Data
	std::vector<Client*> clients;
	std::queue<int>* Client_Connection_Queue;

	// Data
	int MAX_CLIENTS;
	SOCKET server_socket;
	int * GAMESTATE;
	bool* Game_Is_Running;
private:
	// Server Socket Data

	SOCKADDR_IN serverInf;
	std::deque<int>* Game_queue;
	std::queue<std::string>* messages;
	WSADATA WsaDat;
};

