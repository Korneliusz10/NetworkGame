#include "Client_Manager.h"


Client_Manager::Client_Manager(std::queue<std::string>* new_msg_queue, std::queue<std::string>* new_msg_to_server, std::queue<std::string>* new_msg_from_server) {
	this->messages = new_msg_queue;
	this->state = STATE::RESTING;
	this->msg_to_server = new_msg_to_server;
	this->msg_from_server = new_msg_from_server;

	// Creating Client ID
	//--------------------------------
	std::srand(std::time(nullptr)); // use current time as seed for random generator
	Client_ID = std::to_string(std::rand());

}

void Client_Manager::Start_Client() {
	// Initialise Winsock	
	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0) {
		// We have a Error , WSADATA could not be made
		messages->push("Winsock error - Winsock initialization failed");
	}
	else {
		messages->push("Winsock was Initialized correctly");
	}

	// Resolve IP address for hostname.
	struct hostent* host;

	// Change this to point to server, or ip address...

	if ((host = gethostbyname("localhost")) == NULL) {
		messages->push("Failed to resolve hostname");
	}
	else {
		messages->push("Host name was resolved Succesfully");
	}

	// Setup our socket address structure.	
	server_address_size = (int)sizeof(SockAddr);
	SockAddr.sin_port = htons(8888);	// Port number
	SockAddr.sin_family = AF_INET;		// Use the Internet Protocol (IP)
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

	Clear_Memory_For_Buffer(); // set up the buffer
}

void Client_Manager::Creat_Socket() {
	// Create our socket
	main_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (main_socket == INVALID_SOCKET) {
		messages->push("Winsock error - Socket creation Failed!");
	}
	else {
		messages->push("Socket was Created Succesfully");
	}
}

void Client_Manager::Client_cleanup() {

	shutdown(main_socket,SD_SEND);
	closesocket(main_socket);
	WSACleanup();
}

void Client_Manager::Close_Socket() {

	conected = false;
	messages->push("Socket has been Closed");
	shutdown(main_socket, SD_SEND);
	closesocket(main_socket);
}

void Client_Manager::Switch_state(STATE newSTATE) {
	state = newSTATE;
}

STATE Client_Manager::Get_state() {
	return state;
}

// 0 = Blocking | 1 = Non-Blocking
void Client_Manager::Set_Socket_Blocking(int blocking_value) {
	unsigned long blocking_state = static_cast<long>(blocking_value);
	ioctlsocket(main_socket, FIONBIO, &blocking_state);
}

int Client_Manager::Send_State() {
	Set_Socket_Blocking(0);
	strcpy(buffer, msg_to_server->front().c_str()); // Coppy data from somewhere to buffer
	messages->push("Trying to send Data");
	int i_result = sendto(main_socket, buffer, buffer_size, 0, (SOCKADDR*)&SockAddr.sin_addr, server_address_size);
	if (i_result == SOCKET_ERROR) {
		// Error /// Needs more info herer is we will always error out
		messages->push("Message did not send | Connection Error");
		msg_to_server->pop();
		Close_Socket();
		return 0;
	}
	else {
		messages->push("Data was sent");
		msg_to_server->pop();
		return 0;
		// do seomthing here
	}
}

int Client_Manager::Recive_State() {
	Set_Socket_Blocking(0);
	messages->push("Waiting for data");
	int i_recieve = recvfrom(main_socket, buffer, buffer_size, 0, (SOCKADDR*)&SockAddr.sin_addr, &server_address_size); // recieve data
	if (i_recieve == SOCKET_ERROR) {
		// Error /// Needs more info herer is we will always error out
		messages->push("Message did not Recieve | Connection Error");
		//msg_to_server->pop();
		Close_Socket();
		return 0;
	}
	else {
		// we have Recieved correct data
		std::string temp;
		temp = buffer;
		messages->push("Data  was Recievied");
		msg_from_server->push(temp);
		return 0;
		// do something here
	}
}

int Client_Manager::Recive_NB_state() {
	
	//messages->push("Waiting for data");
	int i_recieve = recvfrom(main_socket, buffer, buffer_size, 0, (SOCKADDR*)&SockAddr.sin_addr, &server_address_size); // recieve data
	if (i_recieve == SOCKET_ERROR) {
		// We always get a ERROR now cos the socket is non blocking
		// only time we dont get a error is when we recive good data 
	}	
	else {
		// we have Recieved correct data
		std::string temp;
		temp = buffer;
		//	messages->push("Data  was Recievied");
		msg_from_server->push(temp);
		return 0;
		// do something here
	}
}

int Client_Manager::Handshake_state() {
	Set_Socket_Blocking(0);
	std::string SYN = "SYN";
	std::string ACK;

	msg_to_server->push(SYN);		// Push "SYN" TO MSG_TO_SERVER
	Send_State();					// Send It to Server
	Recive_State();					// Wait for Server TO Send Back The Acknowlagment
	ACK = msg_from_server->front();	// Make ACK = To front of messages from server	
	msg_from_server->pop();			// Get rid of that message
	if (ACK == SYN) {				// If the Message from Server is the same as SYN then we are good
		msg_to_server->push("1");	// Push 1 As Aknowlagment That the data is Correct
		Send_State();				// Send it
		msg_to_server->push(Client_ID);	// Push Cliend ID to the Server
		Send_State();				// Send it
		Switch_state(STATE::RECNONBLOCKING);
	}
	else {						// Server has Send Bad Data
		msg_to_server->push("0");	// Push 0 As Aknowlagment That the data is InCorrect
		Send_State();				// Send it
		Close_Socket();
		Switch_state(STATE::RESTING);
	}

	return 0;
}

void Client_Manager::LookingforConnection_state() {
	Close_Socket(); // close any sockets while looking for connection
	Creat_Socket(); // Creat a new socket with the local class socket
	// now we accept into that sockety
	messages->push("Loocking for Connections");
	if (connect(main_socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0)
	{
		// we did not connect
		messages->push("Connection Faield");
	}
	else {
		// we connected
		messages->push("Connetion was Succesful");
		conected = true;
	}
	// if we faield we can run this again to look for connection and accept a different connection.
}

void Client_Manager::Clear_Memory_For_Buffer() {
	SecureZeroMemory(&buffer, buffer_size);
}