#include "Server_Manager.h"


Server_Manager::Server_Manager(int newMAX_CLIENTS,std::queue<std::string>* newmessages,std::deque<int> * ngame_queue, std::queue<int>* nc_c_q,int * nGAMESTATE,bool*nGame_Is_Runing) {
	this->MAX_CLIENTS = newMAX_CLIENTS;
	this->messages = newmessages;
	this->Game_queue = ngame_queue;
	this->Client_Connection_Queue = nc_c_q;
	this->GAMESTATE = nGAMESTATE;
	this->Game_Is_Running = nGame_Is_Runing;
}

// SERVER STATE FUNCTIONS----------------------------


void Server_Manager::StartServer_state() {
	//WSADATA WsaDat;

	// Initialise Windows Sockets
	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
	{
		messages->push("WSA Initialization failed!");
	}
	else {
		messages->push("WSA Initialization Was Sucessful!");
	}

	// Create a unbound socket.
	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (server_socket == INVALID_SOCKET)
	{
		messages->push("Socket creation failed");
	}
	else {
		messages->push("Server Socket creation was Sucessful");
	}

	// Now, try and bind the socket to any incoming IP address on Port 8888.

	serverInf.sin_family = AF_INET;
	serverInf.sin_addr.s_addr = INADDR_ANY;
	serverInf.sin_port = htons(8888);

	if (bind(server_socket, (SOCKADDR*)(&serverInf), sizeof(serverInf)) == SOCKET_ERROR)
	{
		messages->push("Unable to bind socket!");
	}
	else {
		messages->push("Socket was  binded Sucessfuly!");
	}
}

int Server_Manager::AcceptClient_state(Client * client) {
	
	client->addr_length = sizeof(sockaddr);
	client->socket = accept(server_socket, (sockaddr*)&client->addr, &client->addr_length);

	if (client->socket != 0 && client->socket != SOCKET_ERROR) {
		client->connected = true;
		messages->push("Client Connected : " + std::to_string(client->ID));
		return 0;
	}
	else
	{
		messages->push("Client Failed to Connect");
		return 0;
	}

	return 0;
}


bool Server_Manager::cheack_client_name_exists(std::string client_name) {

	for (int i = 0; i < clients.size(); i++) {
		if (client_name == clients.at(i)->Client_Name) {
			// a client with that name already exists so clean that client
			return true;
		}
		// if not we are good
	}
	return false;
}

void Server_Manager::Listen_state() {
	listen(server_socket, 1);
}

int Server_Manager::Recieve_state(Client* client) {
	//messages->push("Waiting for Data");
	int i_result = recvfrom(client->socket, client->buffer, client->buffer_size, 0, (SOCKADDR*)&client->addr, &client->addr_length);
	if (i_result == SOCKET_ERROR)
	{
		messages->push("Data Recieved is bad | SOCKET ERROR ");
		client->Clear_Buffer_of_Memort();
		Client_Quit(client);
		return 0;
	}
	else {
		std::string temp_s;
		temp_s = client->buffer;
		//messages->push("Data has been Recievied : " + temp_s);
		return 0;
	}
}

int Server_Manager::Send_state(Client * client) {
	messages->push("Waiting to send Data");
	int i_result = sendto(client->socket, client->buffer, client->buffer_size, 0, (SOCKADDR*)&client->addr.sin_addr, client->addr_length);
	if (i_result == SOCKET_ERROR) {
		// Error /// Needs more info herer is we will always error out
		messages->push("Message did not send | Connection Error");
		
		client->Clear_Buffer_of_Memort();
		Client_Quit(client);
		return 0;
	}
	else {
		// We were succesful
		messages->push("Data was sent");
		client->Clear_Buffer_of_Memort();
		return 0;
	}
}

int Server_Manager::HandShake_state(Client * client) {

	Set_blocking(0,&server_socket);
	Set_blocking(0, &client->socket);

	std::string ACK;					// Make ACK
	std::string Temp_s;
	Recieve_state(client);				// Wait for Data from Client
	ACK = client->buffer;				// Get data from Clients buffer and put it into ACK
	client->Clear_Buffer_of_Memort();	// Clear Client Buffer
	strcpy(client->buffer, ACK.c_str());// Push ACK to client
	Send_state(client);					// Send Clients Buffer Data to Client
	Recieve_state(client);				// Wait for Message From Client
	Temp_s = client->buffer;			// Set temp_s to the client buffer
	client->Clear_Buffer_of_Memort();	// Clear Client Buffer
	if(Temp_s == ""){					// Bad Data or Grabage Data Has Came
		messages->push("Client : " + std::to_string(client->ID) + " Has Sent Bad Data");
		Kick_Client(client);
		return 0;
	}
	if (Temp_s == "0") {				// Data Transfer is Not Correct or Sage
		messages->push("Client : " + std::to_string(client->ID) + " Did Not Get Correct Data From US");
		Kick_Client(client);
		return 0;
	}
	if (Temp_s == "1") {				// Data is Correct
		messages->push("Client : " + std::to_string(client->ID) + " Has Sent Acknowlagment of a Safe Connection");
		std::string Client_ID;
		Recieve_state(client);
		Client_ID = client->buffer;
		if (cheack_client_name_exists(Client_ID) == true) { // Cheack If the ID Aleady Exists in out Vector
			Kick_Client(client);
			messages->push("Client with  this name already exists");
			client->Clear_Buffer_of_Memort();
			return 0;
		}
		else {
			messages->push("Client : " + std::to_string(client->ID) + " Has handshaked correctly");
			client->Client_Name = Client_ID;
			client->canbekicked = true;
			client->canlisten = true;
			Client_Connection_Queue->pop();
			client->state = "In Queue";
			Game_queue->push_back(client->ID);
			return 0;
		}
	}
}

// CLIENT FUNCTIONS----------------------------

void Server_Manager::Add_client(int ID) {
	clients.push_back(new Client);			// Add A new client
	clients.at(ID)->ID = ID;				// Give it and ID of Connected
	clients.at(ID)->canlisten = false;		// Cant Listen
	clients.at(ID)->canbekicked = false;	// Cant be kicked
	Client_Connection_Queue->push(clients.at(ID)->ID); // Add to the Accept Queue the ID of this Client
	messages->push("Client : " + std::to_string(clients.at(ID)->ID) + " was Added"); // Print a Message that a Client was Added 
}

void Server_Manager::Clean_Cliens() {
	for (auto  i : clients) {
		// Send Data to clients that They have Disconected
		i->clean_client();
		//------------------------------------------------
		delete i;
	}
	clients.clear();
}

// 0 = Blocking | 1 = Non-Blocking
void Server_Manager::Set_blocking(int blocking_value,SOCKET * client) {
	unsigned long blocking_state = static_cast<long>(blocking_value);
	ioctlsocket(*client, FIONBIO, &blocking_state);
}

void Server_Manager::Kick_Client(Client * client) {

	// Kick the Player from Game Queue

	for (int i = 0; i < Game_queue->size(); i++)
	{
		if (Game_queue->at(i) == client->ID) {
			Game_queue->erase(Game_queue->begin() + i);
		}
	}

	client->canlisten = false;
	std::string quit_msg = "10";
	int ID; // hold temp data
	ID = client->ID;
	messages->push("Client : " + std::to_string(client->ID) + " Has been Disconected");
	strcpy(client->buffer, quit_msg.c_str()); 
	Send_state(client);
	client->Clear_Buffer_of_Memort();
	delete client;
	client = new Client;
	client->ID = ID;
	client->connected = false;
	Client_Connection_Queue->push(client->ID);
}

void Server_Manager::Client_Quit(Client* client) {

	// Kick the Player from Game Queue

	//*GAMESTATE = 5;
	while (*Game_Is_Running == true) {};
	for (int i = 0; i < Game_queue->size(); i++)
	{
		if (Game_queue->at(i) == client->ID) {
			Game_queue->erase(Game_queue->begin() + i);
		}
	}

	client->canlisten = false;
	int ID; // hold temp data
	ID = client->ID;
	client->Clear_Buffer_of_Memort();
	delete client;
	client = new Client;
	client->ID = ID;
	client->connected = false;
	Client_Connection_Queue->push(client->ID);
}

