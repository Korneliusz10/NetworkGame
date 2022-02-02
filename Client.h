#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>


class Client
{
public:

	Client();

	bool connected;		// cheack connection
	sockaddr_in addr;	// addres of the client ( IP )
	SOCKET socket;		// Out Client Socket
	int addr_length;	// other information
	int ID = 0;			// ID to cheack diffrent things
	const int buffer_size = 1024; // data buffer size
	char buffer[1024];	// Char buffer;

	int set_blocking(int);
	void Clear_Buffer_of_Memort();
	void clean_client();

	std::string Client_Name = " ";
	bool canbekicked;
	std::string state;

	bool canlisten = false;


private:
	void Clear_Memeory_for_buffer();
	void Close_Socket();
};

