#include "Client.h"

void Client::Clear_Memeory_for_buffer() {

	SecureZeroMemory(&buffer, buffer_size);
}


Client::Client() {
	Clear_Memeory_for_buffer();
	connected = false;
	canbekicked = false;
	state = "nothing";
	canlisten = false;
}

int Client::set_blocking(int blocking_type) {
	unsigned long t = static_cast<long>(blocking_type);
	return ioctlsocket(socket, FIONBIO, &t);
}

void Client::Clear_Buffer_of_Memort() {
	std::string clear_b = "";
	strcpy(buffer, clear_b.c_str()); // Clear data from buffer by setting it to nothing
}


void Client::Close_Socket() {
	shutdown(socket, SD_SEND);
	closesocket(socket);
}

void Client::clean_client() {
	// Shutdown the socket.
	shutdown(socket, SD_SEND);
	// Close our socket entirely.
	closesocket(socket);
}