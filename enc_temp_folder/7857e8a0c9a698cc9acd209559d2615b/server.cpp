//
// Created by Andrzej on 19.12.2018.
//

#include "server.h"


void server::spakuj() {

	std::string pomocnicza;

	if (z2na10(dlugosc.to_string()) == 10) {
		pomocnicza = operacja.to_string();
		pomocnicza += odpowiedz.to_string();
		pomocnicza += dlugosc.to_string();
		//pomocnicza += wiadomosc.to_string();
		pomocnicza += flagi.to_string();
		pomocnicza += id.to_string();
	}
	else {
		pomocnicza = operacja.to_string();
		pomocnicza += odpowiedz.to_string();
		pomocnicza += dlugosc.to_string();
		pomocnicza += dane.to_string();
		pomocnicza += flagi.to_string();
		pomocnicza += id.to_string();
	}
	buffer_size = pomocnicza.length() / 8;
	std::cout << buffer_size << std::endl;

	for (int i = 0; i < buffer_size; i++) {
		std::string bajt_1 = pomocnicza.substr(i * 8, 8);
		buffer[i] = bit_to_int(bajt_1);
	}
	std::cout << std::endl << buffer << std::endl;

	std::cout << buffer << std::endl;
}


void server::odpakuj() {

	std::string pomocnicza = "";
	/*
	std::string pom = zCna2(buffer[0]);
	operacja = bit_to_int(pom.substr(0, 3));
	odpowiedz = bit_to_int(pom.substr(3, 3));
	pom = zCna2(buffer[5]);
	id = bit_to_int(pom.substr(0, 8));
	*/

	for (int i = 0; i < 1024; i++) {
		pomocnicza += zCna2(buffer[i]);
	}

	std::cout << pomocnicza << std::endl;

		operacja = bit_to_int(pomocnicza.substr(0, 3));
		odpowiedz = bit_to_int(pomocnicza.substr(3, 3));
		dlugosc = bit_to_int(pomocnicza.substr(6, 32));
		dane = bit_to_int(pomocnicza.substr(38, bit_to_int(dlugosc.to_string()) - 10));
		flagi = bit_to_int(pomocnicza.substr(bit_to_int(dlugosc.to_string()) + 28, 2));
		id = bit_to_int(pomocnicza.substr(bit_to_int(dlugosc.to_string()) + 30, 8));

	 /*
	else {
		dlugosc = bit_to_int(pomocnicza.substr(6, 32));
		dane = bit_to_int(pomocnicza.substr(38, bit_to_int(dlugosc.to_string()) - 10));
		flagi = bit_to_int(pomocnicza.substr(bit_to_int(dlugosc.to_string()) + 28, 2));
		id = bit_to_int(pomocnicza.substr(bit_to_int(dlugosc.to_string()) + 30, 8));
	}
	*/
	buffer_size = z2na10(dlugosc.to_string()) / 8;

}

void server::odczytaj() {
	std::string bajt;
	std::string wynik = "";
	std::string pomocnicza = dane.to_string();
	std::cout << pomocnicza;
	for (int i = 0; i < (int)pomocnicza.length(); i = i + 8) {
		bajt = pomocnicza.substr(i, 8);
		wynik = wynik + bit_to_char(bajt);
	}
	std::cout << wynik;
}

int server::bit_to_int(const std::string &s) {
	int liczba = 0;
	for (int i = s.size() - 1, p = 1; i >= 0; i--, p *= 2) {
		if (s[i] == '1')
			liczba += p;
	}
	return liczba;
}

void server::wyczysc() {
	operacja = 0;
	odpowiedz = 0;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
}

void server::zapisz(std::string napis) {
	std::string binarnytekst = "";


	for (int i = 0; i < (int)napis.length(); i++) {
		binarnytekst = binarnytekst + zCna2(napis.at(i));
	}
	dane.reset();
	for (int i = binarnytekst.size() - 1; i >= 0; i--) {
		if (binarnytekst[i] == '1') {
			dane.set(8111 - i, 1);
		}
	}
}


void server::przyjmij_polaczenie() {
	//WYSLANIE INFORMACJI O PRZYJECIU POLACZENIA
	operacja = 0;
	odpowiedz = 1;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
	sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&current_address, sin_size);

}

void server::odrzuc_polaczenie() {
	//WYSLANIE INFORMACJI O ODMOWIE POLACZENIA
	operacja = 0;
	odpowiedz = 2;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
	sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&current_address, sin_size);

}


void server::brak_klienta() {
	//WYSLANIE ZADANIA POLACZENIA
	operacja = 1;
	odpowiedz = 3;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
	sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&current_address, sin_size);

}

void server::odmow_wyslania() {
	//WYSLANIE ZADANIA POLACZENIA
	operacja = 2;
	odpowiedz = 2;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
	sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&current_address, sin_size);

}

void server::potwierdz_zakonczenie() {
	//WYSLANIE ZADANIA POLACZENIA
	operacja = 3;
	odpowiedz = 1;
	dlugosc = 0;
	dane = 0;
	flagi = 0;
	spakuj();
	sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&current_address, sin_size);


}

void server::zadaj_zakonczenia() {
	//ZADANIE ZAKONCZENIA SESJI
	operacja = 3;
	odpowiedz = 0;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();

}

void server::odrzuc_zakonczenie() {
	operacja = 3;
	odpowiedz = 4;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
}


void server::wymus_koniec()
{
	operacja = 7;
	odpowiedz = 7;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
}

int server::UDP()
{
	
	int wsResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsResult != 0)
	{
		std::cout << "Can't initialize winsock! Quiting" << std::endl;
		
	}

	server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (server_socket == INVALID_SOCKET)
	{
		std::cout << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		
	}


	my_address.sin_family = AF_INET;
	my_address.sin_port = htons(55554);
	my_address.sin_addr.S_un.S_addr = INADDR_ANY;

	//inet_pton(AF_INET, "127.0.0.1", &my_address.sin_addr);

	if (bind(server_socket, (sockaddr*)&my_address, sizeof(my_address)) == SOCKET_ERROR) {
		std::cout << "Cant bind socket. Quiting!" << std::endl;
		closesocket(server_socket);
		WSACleanup();
		
	}

	bind(server_socket, (sockaddr*)&my_address, sizeof(my_address));
	std::cout << "Zbindowano" << std::endl;

	
	while (true)
	{
		recvfrom(server_socket, buffer, 1024, 0, (struct sockaddr *) &current_address, &sin_size);
		odpakuj();

		if (operacja == 0 && odpowiedz == 0) {
			std::cout << "<----uzytkownik probuje sie polaczyc---->\n";
			if (users_counter == 0) {
				client1_address = current_address;
				users_counter++;
				wyczysc();
				srand(time(NULL));
				id = 254;
				std::cout << "<----wygenerowano id sesji---->\nID: " << id << std::endl;
				przyjmij_polaczenie();
				//sendto(server_socket, buffer, sizeof(buffer), 0, (sockaddr*)&current_address, sin_size);
				std::cout << "<----zezwolono na polaczenie uzytkownikowi #01---->\n";
			}
			else {
				if (users_counter == 1) {
					if (current_address.sin_port != client1_address.sin_port) {
						client2_address = current_address;
						users_counter++;
						wyczysc();
						przyjmij_polaczenie();
						//sendto(server_socket, buffer, 8, 0, (sockaddr*)&client2_address, sin_size);
						std::cout << "<----zezwolono na polaczenie uzytkownikowi #02---->\n";
					}
				}
				else {
					users_counter++;
					wyczysc();
					odrzuc_polaczenie();
					//sendto(server_socket, buffer, 8, 0, (sockaddr*)&current_address, sin_size);
					std::cout << "<----nie zezwolono na polaczenie uzytkownikowi---->\n";
				}
			}
		}


		if (operacja == 1 && odpowiedz == 0) {
			std::cout << "<----uzytkownik probuje zaprosic innego uzytkownika do rozmowy---->\n";
			if (users_counter == 1) {
				std::cout << "<----uzytkownik proboje zaprosic nieobecnego uzytkownika---->\n";
				wyczysc();
				brak_klienta();
			}
			if (users_counter > 1) {
				std::cout << "<----zaproszono drugiego uzytkownika---->\n";
				if (current_address.sin_port == client1_address.sin_port) {
					odpakuj();
					std::cout << "<SERVER INFOLOG> user #01 --> server: ";
					odczytaj();
					//int rozmiar = 0;
					//std::string roz = dlugosc.to_string();
					//rozmiar = z2na10(roz.substr(0, 8)) * 1000 + z2na10(roz.substr(8, 8)) * 100 + z2na10(roz.substr(16, 8)) * 10 + z2na10(roz.substr(24, 8)) * 1;
					//buffer_size = rozmiar;
					sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);
					std::cout << "<SERVER INFOLOG> server --> user #02: ";
					odczytaj();
					wyczysc();
				}
				else {
					odpakuj();
					std::cout << "<SERVER INFOLOG> user #02 --> server: ";
					odczytaj();
					//int rozmiar = 0;
					//std::string roz = dlugosc.to_string();
					//rozmiar = z2na10(roz.substr(0, 8)) * 1000 + z2na10(roz.substr(8, 8)) * 100 + z2na10(roz.substr(16, 8)) * 10 + z2na10(roz.substr(24, 8)) * 1;
					//buffer_size = rozmiar;
					sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
					std::cout << "<SERVER INFOLOG> server --> user #01: ";
					odczytaj();
					wyczysc();
				}
			}



			else {

				std::cout << "<----za malo uzytkownikow--->\n";
				wyczysc();
				brak_klienta();
				//sendto(server_socket, buffer, buffer_size / 8, 0, (sockaddr*)&current_address, sin_size);
			}
		}


		if (operacja == 1 && odpowiedz == 1) {
			if (current_address.sin_port == client1_address.sin_port) {
				odpakuj();
				std::cout << "<SERVER INFOLOG> user #01 --> server: ";
				odczytaj();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #02: ";
				odczytaj();
			}
			else {
				odpakuj();
				std::cout << "<SERVER INFOLOG> user #02 --> server: ";
				odczytaj();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #01: ";
				odczytaj();
			}
		}

		if (operacja == 1 && odpowiedz == 2) {

			if (current_address.sin_port == client1_address.sin_port) {
				odpakuj();
				std::cout << "<SERVER INFOLOG> user #01 --> server: ";
				odczytaj();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #02: ";
				odczytaj();
			}
			else {
				odpakuj();
				std::cout << "<SERVER INFOLOG> user #02 --> server: ";
				odczytaj();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #01: ";
				odczytaj();
			}
		}

		if (operacja == 1 && odpowiedz == 3) {

			std::cout << "13\n";
		}

		if (operacja == 2 && odpowiedz == 0) {

			if (current_address.sin_port == client1_address.sin_port) {
				odpakuj();
				std::cout << "<SERVER INFOLOG> user #01 --> server: ";
				odczytaj();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #02: ";
				odczytaj();
			}
			else {
				odpakuj();
				std::cout << "<SERVER INFOLOG> user #02 --> server: ";
				odczytaj();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #01: ";
				odczytaj();
			}
		}

		if (operacja == 2 && odpowiedz == 1) {

			if (current_address.sin_port == client1_address.sin_port) {
				;
				std::cout << "<SERVER INFOLOG> user #01 --> server: ";
				odczytaj();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #02: ";
				odczytaj();
			}
			else {
				std::cout << "<SERVER INFOLOG> user #02 --> server: ";
				odczytaj();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #01: ";
				odczytaj();
			}
		}

		if (operacja == 2 && odpowiedz == 2) {
			std::cout << "22\n";
		}

		if (operacja == 2 && odpowiedz == 7) {
			std::cout << "<----dostarczono komunikat---->\n";
		}

		if (operacja == 3 && odpowiedz == 0) {

			std::cout << "<----uzytkownik zada zakonczenia sesji--->\n";
			if (current_address.sin_port == client1_address.sin_port) {

				zadaj_zakonczenia();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);

			}
			else {
				zadaj_zakonczenia();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
			}
		}

		if (operacja == 3 && odpowiedz == 1) {
			std::cout << "<----zezwolono na rozlaczenie--->\n";
			std::cout << "<----trwa rozlaczanie--->\n";
			if (current_address.sin_port == client1_address.sin_port) {

				potwierdz_zakonczenie();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);

			}
			else {
				potwierdz_zakonczenie();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
			}
			break;
		}

		if (operacja == 3 && odpowiedz == 4) {

			if (current_address.sin_port == client1_address.sin_port) {
				odrzuc_zakonczenie();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);

			}
			else {
				odrzuc_zakonczenie();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
			}

		}

		if (operacja == 7 && odpowiedz == 7) {

			if (current_address.sin_port == client1_address.sin_port) {
				wymus_koniec();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);

			}
			else {
				wymus_koniec();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
			}
			break;

		}

	}
	return 1;
}