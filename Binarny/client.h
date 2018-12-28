#pragma once

#include <conio.h>
#include <iostream>
#include <thread>
#include <string>
#include <bitset>
#include <winsock2.h>
#include <ws2tcpip.h>


#pragma comment(lib, "ws2_32.lib")

class client
{
private:

	WSADATA wsaData;
	SOCKET client_socket;
	SOCKADDR_IN dest_addr;

	char buffer[1024];
	int sin_size = sizeof(sockaddr_in);
	int identyfikator;
	std::string text = "";
	int buffer_size;


	std::bitset<3> operacja;
	std::bitset<3> odpowiedz;
	std::bitset<32> dlugosc;
	std::bitset<8144> dane;
	std::string danestr = "";
	std::bitset<2> flagi;
	std::bitset<8> id;


	void ponow();
	void odbierz_wiadomosc(SOCKET client_socket, char buffer[1024], SOCKADDR_IN dest_addr, int sin_size);
	void rozmowa();
	void zapisz(std::string napis);
	void spakuj();
	void odpakuj();
	void odczytaj();
	void wyczysc();
	void zadaj_polaczenia();
	void zadaj_zaproszenia();
	void przyjmij_zaproszenie();
	void odrzuc_zaproszenie();
	void zadaj_wyslania();
	void potwierdzenie();
	void potw();
	void zadaj_zakonczenia();
	void potwierdz_zakonczenie();
	void odrzuc_zakonczenie();
	void wymus_koniec();

public:
	int UDP();
};
