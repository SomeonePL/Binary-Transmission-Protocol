//
// Created by Andrzej on 19.12.2018.
//

#include "server.h"


void server::spakuj()
{
	std::string pomocnicza;

	if (z2na10(dlugosc.to_string()) == 10)
	{
		pomocnicza = operacja.to_string();
		pomocnicza += odpowiedz.to_string();
		pomocnicza += dlugosc.to_string();

		pomocnicza += flagi.to_string();
		pomocnicza += id.to_string();
	}
	else
	{
		pomocnicza = operacja.to_string();
		pomocnicza += odpowiedz.to_string();
		pomocnicza += dlugosc.to_string();
		pomocnicza += dane.to_string().substr(0, z2na10(dlugosc.to_string()) - 10);
		pomocnicza += flagi.to_string();
		pomocnicza += id.to_string();
	}
	buffer_size = pomocnicza.length() / 8;
	std::cout << buffer_size << std::endl;
	ZeroMemory(buffer, 1024);
	for (int i = 0; i < buffer_size; i++)
	{
		buffer[i] = std::stoi(pomocnicza.substr(i * 8, 8), nullptr, 2);
	}
	//std::cout << std::endl << pomocnicza << std::endl;
}


void server::odpakuj()
{
	std::string pomocnicza = "";

	for (int i = 0; i < 1024; i++)
	{
		pomocnicza += zCna2(buffer[i]);
	}


	operacja = std::stoi(pomocnicza.substr(0, 3), nullptr, 2);
	odpowiedz = std::stoi(pomocnicza.substr(3, 3), nullptr, 2);
	dlugosc = std::stoi(pomocnicza.substr(6, 32), nullptr, 2);
	int rozm = std::stoi(dlugosc.to_string(), nullptr, 2) - 10;
	if (rozm > 0)
	{
		for (int i = 0; i < rozm / 8; i += 1)
		{
			danestr.push_back((char)std::stoi(pomocnicza.substr(38 + (i * 8), 8), nullptr, 2));
		}
	}


	flagi = std::stoi(pomocnicza.substr(38 + std::stoi(dlugosc.to_string(), nullptr, 2) - 10, 2), nullptr, 2);
	id = std::stoi(pomocnicza.substr(40 + std::stoi(dlugosc.to_string(), nullptr, 2) - 10, 8), nullptr, 2);
	buffer_size = (z2na10(dlugosc.to_string()) + 38) / 8;
}

void server::odczytaj()
{
	std::string bajt;
	std::string wynik = "";
	std::string pomocnicza = dane.to_string();
	//std::cout << pomocnicza;
	for (int i = 0; i < (int)pomocnicza.length(); i = i + 8)
	{
		bajt = pomocnicza.substr(i, 8);
		wynik = wynik + bit_to_char(bajt);
	}
	std::cout << wynik;
}


void server::wyczysc()
{
	operacja = 0;
	odpowiedz = 0;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
}

void server::zapisz(std::string napis)
{
	std::string binarnytekst = "";


	for (int i = 0; i < (int)napis.length(); i++)
	{
		binarnytekst = binarnytekst + zCna2(napis.at(i));
	}
	dane.reset();
	for (int i = binarnytekst.size() - 1; i >= 0; i--)
	{
		if (binarnytekst[i] == '1')
		{
			dane.set(8111 - i, true);
		}
	}
}


void server::przyjmij_polaczenie()
{
	//WYSLANIE INFORMACJI O PRZYJECIU POLACZENIA
	operacja = 0;
	odpowiedz = 1;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
	sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&current_address, sin_size);
}

void server::odrzuc_polaczenie()
{
	//WYSLANIE INFORMACJI O ODMOWIE POLACZENIA
	operacja = 0;
	odpowiedz = 2;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
	sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&current_address, sin_size);
}


void server::brak_klienta()
{
	//WYSLANIE ZADANIA POLACZENIA
	operacja = 1;
	odpowiedz = 3;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
	sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&current_address, sin_size);
}

void server::odmow_wyslania()
{
	//WYSLANIE ZADANIA POLACZENIA
	operacja = 2;
	odpowiedz = 2;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
	sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&current_address, sin_size);
}

void server::potwierdz_zakonczenie()
{
	//WYSLANIE ZADANIA POLACZENIA
	operacja = 3;
	odpowiedz = 1;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
	sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&current_address, sin_size);
}

void server::zadaj_zakonczenia()
{
	//ZADANIE ZAKONCZENIA SESJI
	operacja = 3;
	odpowiedz = 0;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
}

void server::odrzuc_zakonczenie()
{
	operacja = 3;
	odpowiedz = 4;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
}

void server::potwierdzenie()
{
	//POTWIERDZENIE OTRZYMANIA WIADOMOSCI
	operacja = 2;
	odpowiedz = 1;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
}


void server::wymus_koniec()
{
	//ZERWANIE POLACZENIA
	operacja = 7;
	odpowiedz = 7;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	spakuj();
}

int server::UDP()
{
	int iden;

	WSAStartup(MAKEWORD(2, 2), &wsaData); // Inicjalizacja WinSock

	my_address.sin_family = AF_INET;
	my_address.sin_port = htons(55555); // przypisanie portu [55555]
	inet_pton(AF_INET, "127.0.0.1", &my_address.sin_addr); //przypisanie IP [127.0.0.1]

	server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //tworzenie socketu
	bind(server_socket, (sockaddr*)&my_address, sizeof(my_address)); //przypisanie socketu
	std::cout << "Zbindowano" << std::endl;


	while (true)
	{
		recvfrom(server_socket, buffer, 1024, 0, (struct sockaddr *)&current_address, &sin_size);
		odpakuj();

		if (operacja == 0 && odpowiedz == 0)
		{
			potwierdzenie();
			sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&current_address, sin_size);
			std::cout << "<----uzytkownik probuje sie polaczyc---->\n";
			if (users_counter == 0)
			{
				client1_address = current_address;
				users_counter++;
				wyczysc();
				srand(time(nullptr));
				id = 254;
				iden = 254;
				std::cout << "<----wygenerowano id sesji---->\nID: " << id << std::endl;
				przyjmij_polaczenie();
				std::cout << "<----zezwolono na polaczenie uzytkownikowi #01---->\n";
			}
			else
			{
				if (users_counter == 1)
				{
					if (current_address.sin_port != client1_address.sin_port)
					{
						client2_address = current_address;
						users_counter++;
						wyczysc();
						id = iden;
						przyjmij_polaczenie();
						std::cout << "<----zezwolono na polaczenie uzytkownikowi #02---->\n";
					}
				}
				else
				{
					wyczysc();
					odrzuc_polaczenie();
					std::cout << "<----nie zezwolono na polaczenie uzytkownikowi---->\n";
				}
			}
		}


		if (operacja == 1 && odpowiedz == 0)
		{
			std::cout << "<----uzytkownik probuje zaprosic innego uzytkownika do rozmowy---->\n";
			if (users_counter == 1)
			{
				std::cout << "<----uzytkownik proboje zaprosic nieobecnego uzytkownika---->\n";
				wyczysc();
				brak_klienta();
			}
			if (users_counter > 1)
			{
				std::cout << "<----zaproszono drugiego uzytkownika---->\n";
				if (current_address.sin_port == client1_address.sin_port)
				{
					std::cout << "<SERVER INFOLOG> user #01 --> server: ";
					sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);
					std::cout << "<SERVER INFOLOG> server --> user #02: " << std::endl;
					//odczytaj();
					wyczysc();
				}
				else
				{
					std::cout << "<SERVER INFOLOG> user #02 --> server: ";
					sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
					std::cout << "<SERVER INFOLOG> server --> user #01: " << std::endl;
					wyczysc();
				}
			}


			else
			{
				std::cout << "<----za malo uzytkownikow--->\n";
				wyczysc();
				brak_klienta();
			}
		}


		if (operacja == 1 && odpowiedz == 1)
		{
			if (current_address.sin_port == client1_address.sin_port)
			{
				std::cout << "<SERVER INFOLOG> user #01 --> server: ";
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #02: " << std::endl;
			}
			else
			{
				std::cout << "<SERVER INFOLOG> user #02 --> server: ";
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #01: " << std::endl;
			}
		}

		if (operacja == 1 && odpowiedz == 2)
		{
			if (current_address.sin_port == client1_address.sin_port)
			{
				std::cout << "<SERVER INFOLOG> user #01 --> server: ";
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #02: " << std::endl;
			}
			else
			{
				std::cout << "<SERVER INFOLOG> user #02 --> server: ";
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #01: " << std::endl;
			}
		}

		if (operacja == 2 && odpowiedz == 0)
		{
			if (current_address.sin_port == client1_address.sin_port)
			{
				std::cout << "<SERVER INFOLOG> user #01 --> server: ";
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #02: " << std::endl;
			}
			else
			{
				std::cout << "<SERVER INFOLOG> user #02 --> server: ";
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #01: " << std::endl;
			}
		}

		if (operacja == 2 && odpowiedz == 1)
		{
			if (current_address.sin_port == client1_address.sin_port)
			{
				std::cout << "<SERVER INFOLOG> user #01 --> server: ";
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #02: " << std::endl;
			}
			else
			{
				std::cout << "<SERVER INFOLOG> user #02 --> server: ";
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
				std::cout << "<SERVER INFOLOG> server --> user #01: " << std::endl;
			}
		}

		if (operacja == 2 && odpowiedz == 7)
		{
			std::cout << "<----dostarczono komunikat---->\n";
		}

		if (operacja == 3 && odpowiedz == 0)
		{
			std::cout << "<----uzytkownik zada zakonczenia sesji--->\n";
			if (current_address.sin_port == client1_address.sin_port)
			{
				zadaj_zakonczenia();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);
			}
			else
			{
				zadaj_zakonczenia();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
			}
		}

		if (operacja == 3 && odpowiedz == 1)
		{
			std::cout << "<----zezwolono na rozlaczenie--->\n";
			std::cout << "<----trwa rozlaczanie--->\n";
			if (current_address.sin_port == client1_address.sin_port)
			{
				potwierdz_zakonczenie();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);
			}
			else
			{
				potwierdz_zakonczenie();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
			}
			break;
		}

		if (operacja == 3 && odpowiedz == 4)
		{
			if (current_address.sin_port == client1_address.sin_port)
			{
				odrzuc_zakonczenie();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);
			}
			else
			{
				odrzuc_zakonczenie();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
			}
		}

		if (operacja == 7 && odpowiedz == 7)
		{
			if (current_address.sin_port == client1_address.sin_port)
			{
				wymus_koniec();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client2_address, sin_size);
			}
			else
			{
				wymus_koniec();
				sendto(server_socket, buffer, buffer_size, 0, (sockaddr*)&client1_address, sin_size);
			}
			break;
		}
	}
	return 1;
}
