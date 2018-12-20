#pragma once
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
#include <thread>
#include <bitset>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "converters.h"

#pragma comment(lib, "Ws2_32.lib")

class server {
private:
	WSADATA wsaData;
	SOCKET server_socket;
	SOCKADDR_IN my_address, current_address;
	SOCKADDR_IN client1_address, client2_address;
	SOCKET soc;

	int sin_size = sizeof(sockaddr_in);
	char buffer[1024];
	int users_counter = 0;
	std::string text;
	int buffer_size;

	std::bitset<3> operacja;
	std::bitset<3> odpowiedz;
	std::bitset<32> dlugosc;
	std::bitset<8112> dane;
	std::bitset<2> flagi;
	std::bitset<8> id;

	void spakuj();
	void odpakuj();
	void odczytaj();
	void wyczysc();
	int bit_to_int(const std::string &s);

	void zapisz(std::string napis);
	void przyjmij_polaczenie();
	void odrzuc_polaczenie();
	void brak_klienta();
	void odmow_wyslania();
	void potwierdz_zakonczenie();
	void zadaj_zakonczenia();
	void odrzuc_zakonczenie();
	void wymus_koniec();

public:
	int UDP();
};

