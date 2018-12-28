//
// Created by Andrzej on 19.12.2018.
//


#include <cmath>
#include "client.h"
#include "converters.h"


//KOMUNIKATY

void client::wyczysc()
{
	operacja = 0;
	odpowiedz = 0;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	id = identyfikator;
}

void client::zadaj_polaczenia()
{
	//ZADANIE NAWIAZANIA POLACZENIA
	operacja = 0;
	odpowiedz = 0;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	id = 0;
	spakuj();
}


void client::zadaj_zaproszenia()
{
	//WYSLANIE ZAPROSZENIA
	operacja = 1;
	odpowiedz = 0;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	id = identyfikator;
	spakuj();
}

void client::przyjmij_zaproszenie()
{
	//PRZYJECIE ZAPROSZENIA
	operacja = 1;
	odpowiedz = 1;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	id = identyfikator;
	spakuj();
}

void client::odrzuc_zaproszenie()
{
	//ODRZUCENIE ZAPROSZENIA
	operacja = 1;
	odpowiedz = 2;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	id = identyfikator;
	spakuj();
}

void client::potwierdzenie()
{
	//POTWIERDZENIE OTRZYMANIA WIADOMOSCI
	operacja = 2;
	odpowiedz = 1;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	id = identyfikator;
	spakuj();
}

void client::zadaj_zakonczenia()
{
	//ZADANIE ZAKONCZENIA SESJI
	operacja = 3;
	odpowiedz = 0;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	id = identyfikator;
	spakuj();
}

void client::potwierdz_zakonczenie()
{
	//POTWIERDZENIE OTRZYMANIA WIADOMOSCI
	operacja = 3;
	odpowiedz = 1;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	id = identyfikator;
	spakuj();
}

void client::ponow()
{
	std::cout << "Co chcesz zrobic?\n[1] Sprobuj ponownie\n[2] Rozlacz\n";
	int reakcja;
	reakcja = _getch();
	if (reakcja == '1')
	{
		std::cout << "<--trwa proba ponownego polaczenia-->\n";
		wyczysc();
		zadaj_polaczenia();
		if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
		{
			std::cout << "Blad sendto nr" << errno << std::endl;
		}
		wyczysc();
	}
	if (reakcja == '2')
	{
		std::cout << "<--zadanie zakonczenia polaczenia-->\n";
		wyczysc();
		zadaj_zakonczenia();
		if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
		{
			std::cout << "Blad sendto nr" << errno << std::endl;
		}
		wyczysc();
	}
	else
	{
		wyczysc();
		Sleep(100);
	}
}

void client::odrzuc_zakonczenie()
{
	operacja = 3;
	odpowiedz = 4;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	id = identyfikator;
	spakuj();
}

void client::wymus_koniec()
{
	operacja = 7;
	odpowiedz = 7;
	dlugosc = 10;
	dane = 0;
	flagi = 0;
	id = identyfikator;
	spakuj();
}


void client::zadaj_wyslania()
{
	//WYSLANIE WIADOMOSCI
	getline(std::cin, text);
	if (text == "#")
	{
		zadaj_zakonczenia();

		if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
		{
			std::cout << "Blad sendto nr" << errno << std::endl;
		}
		wyczysc();
		return;
	}
	if (text == "###")
	{
		wymus_koniec();
		if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
		{
			std::cout << "Blad sendto nr" << errno << std::endl;
		}
	}
	else
	{
		operacja = 2;
		odpowiedz = 0;
		int d = text.size() * 8 + 10; //rozmiar


		if (d <= 1019 * 8)
		{
			flagi = 0;
			id = identyfikator;
			dlugosc = d;
			zapisz(text);
			spakuj();

			if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
			{
				std::cout << "Blad sendto nr" << errno << std::endl;
			}
		}
		else
		{
			float paczki = d / (1019 * 8);
			paczki = ceil(paczki);

			int wyslane = 0;


			for (int i = 0; i < paczki - 1; i++)
			{
				flagi = 3;
				id = identyfikator;
				dlugosc = 1019;
				zapisz(text.substr(i * (1019), 1019));
				wyslane++;

				spakuj();
				if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
				{
					std::cout << "Blad sendto nr" << errno << std::endl;
				}


				while (operacja != 2 && odpowiedz != 7)
				{
				}
			}

			std::cout << std::endl << wyslane << "< wyslane " << paczki << " < paczki";
			flagi = 2;
			id = identyfikator;


			int temp = d - (wyslane * 1019 * 8);
			zapisz(text.substr((wyslane + 1) * (1019), temp / 8));

			spakuj();
			wyslane++;
			if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
			{
				std::cout << "Blad sendto nr" << errno << std::endl;
			}

			std::cout << std::endl << wyslane << "< wyslane " << paczki << " < paczki";
		}
	}
}

//

//OPERACJE NA DANYCH

void client::spakuj()
{
	std::string pomocnicza = "";

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

void client::zapisz(std::string napis)
{
	std::string binarnytekst = "";


	for (int i = 0; i < (int)napis.size(); i++)
	{
		binarnytekst = binarnytekst + zCna2(napis[i]);
	}

	dane = 0;
	for (int i = binarnytekst.size() - 1; i >= 0; i--)
	{
		if (binarnytekst[i] == '1')
		{
			dane.set(8143 - i, true);
		}
	}
}


void client::odpakuj()
{
	std::string pomocnicza = "";

	for (int i = 0; i < 1024; i++)
	{
		pomocnicza += zCna2(buffer[i]);
	}

	std::string s = "";

	operacja = std::stoi(pomocnicza.substr(0, 3), nullptr, 2);
	odpowiedz = std::stoi(pomocnicza.substr(3, 3));
	dlugosc = std::stoi(pomocnicza.substr(6, 32), nullptr, 2);


	int rozm = std::stoi(dlugosc.to_string(), nullptr, 2) - 10;
	unsigned int bitset = 0;
	if (rozm > 0)
	{
		for (int i = 0; i < rozm / 8; i++)
		{
			danestr.push_back((char)std::stoi(pomocnicza.substr(38 + (i * 8), 8), nullptr, 2));
		}
	}


	flagi = std::stoi(pomocnicza.substr(38 + std::stoi(dlugosc.to_string(), nullptr, 2) - 10, 2), nullptr, 2);
	id = std::stoi(pomocnicza.substr(40 + std::stoi(dlugosc.to_string(), nullptr, 2) - 10, 8), nullptr, 2);


	//std::cout << "otrzymano :" << std::endl << " dl " << z2na10(dlugosc.to_string()) << std::endl;
}


void client::odczytaj()
{
	std::string bajt;
	std::string wynik = "";
	std::string pomocnicza = dane.to_string();
	std::cout << pomocnicza;
	for (int i = 0; i < (int)pomocnicza.length(); i = i + 8)
	{
		bajt = pomocnicza.substr(i, 8);
		wynik = wynik + bit_to_char(bajt);
	}
	std::cout << wynik;
}


//KLIENT

int client::UDP()
{
	//WINSOCK
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(55555);
	inet_pton(AF_INET, "127.0.0.1", &dest_addr.sin_addr);

	std::cout << "<---proba nawiazania polaczenia--->\n";
	zadaj_polaczenia();
	sendto(client_socket, buffer, buffer_size, 0, (sockaddr*)&dest_addr, sin_size);

	std::cout << "<-wyslano->\n";
	wyczysc();

	std::thread ODBIOR(&client::odbierz_wiadomosc, this, client_socket, buffer, dest_addr, sin_size);
	ODBIOR.detach();

	while (true)
	{
		while (operacja == 0 && odpowiedz == 0)
		{
		}


		if (operacja == 0 && odpowiedz == 1)
		{
			std::cout << "<----nawiazano polaczenie---->\n";
			std::cout << "<----otrzymano id sesji---->\n";
			identyfikator = z2na10(id.to_string());
			std::cout << "Co chcesz zrobic?\n[1] Zapros do rozmowy\n[2] Czekaj\n";
			int reakcja;
			reakcja = _getch();
			if (reakcja == '1')
			{
				std::cout << "<----trwa zapraszanie innego uytkownika do rozmowy---->\n";
				wyczysc();
				zadaj_zaproszenia();
				if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
				{
					std::cout << "Blad sendto nr" << errno << std::endl;
				}
				while (operacja == 1 && odpowiedz == 0)
				{
				}
			}
			if (reakcja == '2')
			{
				std::cout << "<----oczekuje na zaproszenie---->\n";
				wyczysc();
			}
		}

		if (operacja == 0 && odpowiedz == 2)
		{
			std::cout << "<----odmowa nawiazania polaczenia---->\n";
			ponow();
		}

		if (operacja == 0 && odpowiedz == 3)
		{
			std::cout << "<----problem po stronie serwera lub serwer nieosiagalny---->\n";
			ponow();
		}

		if (operacja == 1 && odpowiedz == 0)
		{
			std::cout << "<----otrzymano zaproszenie do rozmowy---->\n";
			std::cout << "Co chcesz zrobic?\n[1] Przyjmij\n[2] Odrzuc\n[3] Rozlacz - nieaktywne\n";
			int reakcja;
			reakcja = _getch();
			if (reakcja == '1')
			{
				std::cout << "<--przyjmowanie zaproszenia-->\n";
				wyczysc();
				przyjmij_zaproszenie();
				if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
				{
					std::cout << "Blad sendto nr" << errno << std::endl;
				}
				rozmowa();
			}
			if (reakcja == '2')
			{
				std::cout << "<----odrzucanie zaproszenia---->\n";
				wyczysc();
				odrzuc_zaproszenie();
				if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
				{
					std::cout << "Blad sendto nr" << errno << std::endl;
				}
				wyczysc();
			}
		}


		if (operacja == 1 && odpowiedz == 1)
		{
			std::cout << "<----uzytkownik przyjal zaproszenie---->\n";
			rozmowa();
		}

		if (operacja == 1 && odpowiedz == 2)
		{
			std::cout << "<----uzytkownik odrzucil zaproszenie---->\n";
			std::cout << "Co chcesz zrobic?\n[1] Zapros ponownie\n[2] Rozlacz\n";
			int reakcja;
			reakcja = _getch();
			if (reakcja == '1')
			{
				std::cout << "<----trwa ponowne zapraszaine do rozmowy---->\n";
				wyczysc();
				zadaj_zaproszenia();
				if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
				{
					std::cout << "Blad sendto nr" << errno << std::endl;
				}
			}
			if (reakcja == '2')
			{
				std::cout << "<----zakonczenie polaczenia---->\n";
				wyczysc();
				potwierdz_zakonczenie();
				if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
				{
					std::cout << "Blad sendto nr" << errno << std::endl;
				}
			}
		}

		if (operacja == 1 && odpowiedz == 3)
		{
			std::cout << "<----nie udalo sie zaprosic uzytkownika---->\n";
			std::cout << "Co chcesz zrobic?\n[1] Sprobuj ponownie\n[2] Rozlacz\n";
			int reakcja;
			reakcja = _getch();
			if (reakcja == '1')
			{
				std::cout << "<----trwa proba ponownego zaproszenia uzytkownika---->\n";
				wyczysc();
				zadaj_zaproszenia();
				if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
				{
					std::cout << "Blad sendto nr" << errno << std::endl;
				}
			}
			if (reakcja == '2')
			{
				std::cout << "<----zadanie zakonczenia polaczenia---->\n";
				wyczysc();
				zadaj_zakonczenia();
				if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
				{
					std::cout << "Blad sendto nr" << errno << std::endl;
				}
				wyczysc();
			}
		}

		if (operacja == 2 && odpowiedz == 7)
		{
			std::cout << "<----dostarczono komunikat---->\n";
		}

		if (operacja == 3 && odpowiedz == 0)
		{
			std::cout << "<----rozmowca chce zakonczyc sesje---->\n[1] Pozwol odejsc\n[2] Utrzymaj polaczenie\n";
			int reakcja;
			reakcja = _getch();
			if (reakcja == '1')
			{
				std::cout << "<----zezwolono na rozlaczenie---->\n";
				potwierdz_zakonczenie();
				if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
				{
					std::cout << "Blad sendto nr" << errno << std::endl;
				}
				break;
			}
			if (reakcja == '2')
			{
				std::cout << "<----wyslano komunikat o checi utrzymania polaczenia---->\n";

				wyczysc();
			}
		}

		if (operacja == 3 && odpowiedz == 1)
		{
			std::cout << "<----zezwolono na rozlaczenie--->\n";
			std::cout << "<----trwa rozlaczanie--->\n";
			break;
		}

		if (operacja == 7 && odpowiedz == 7)
		{
			std::cout << "<---- wymuszono zakonczenie sesji---->\n";
			std::cout << "<----rozlaczono---->\n";
			break;
		}

		int i = sizeof(dest_addr);
	}

	closesocket(client_socket);
	WSACleanup();
	std::cout << "<----rozlaczono---->\n";
	return 1;
}

void client::odbierz_wiadomosc(SOCKET client_socket, char buffer[1024], SOCKADDR_IN dest_addr, int sin_size)
{
	while (true)
	{
		recvfrom(client_socket, buffer, 1024, 0, (sockaddr*)&dest_addr, &sin_size);
		odpakuj();

		if (operacja == 2 && odpowiedz == 1)
		{
			std::cout << "<----dostarczono--->\n";
			wyczysc();
		}

		if (operacja == 2 && odpowiedz == 0)
		{
			std::string pom = "";

			std::cout << "\nRozmowca napisal: " << std::endl << danestr;
			danestr = "";
			std::cout << std::endl;


			potwierdzenie();
			if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
			{
				std::cout << "Blad sendto nr" << errno << std::endl;
			}
			wyczysc();


			std::cout << std::endl;

			if (flagi == 3)
			{
				do
				{
					recvfrom(client_socket, buffer, 1024, 0, (sockaddr*)&dest_addr, &sin_size);
					odpakuj();
					std::cout << "\nRozmowca napisal: " << std::endl;

					potwierdzenie();
					if (sendto(client_socket, buffer, buffer_size, 0, (sockaddr *)&dest_addr, sin_size) == -1)
					{
						std::cout << "Blad sendto nr" << errno << std::endl;
					}
					wyczysc();

					std::cout << std::endl;
				}
				while (flagi == 3);
			}
		}
		Sleep(100);
	}
}


void client::rozmowa()
{
	std::cout << "Przywitaj sie! Napisz\"hej\"\nAby zakonczyc rozmowe wpisz #\n";
	while (true)
	{
		zadaj_wyslania();
		wyczysc();

		while (operacja == 0 && odpowiedz == 0)
		{
		}

		if (operacja == 3 && odpowiedz == 0)
		{
			std::cout << "<----rozmowca chce zakonczyc sesje---->\n[1] Pozwol odejsc\n[2] Utrzymaj polaczenie\n";
			int reakcja;
			reakcja = _getch();
			if (reakcja == '1')
			{
				std::cout << "<----zezwolono na rozlaczenie--->\n";
				potwierdz_zakonczenie();
				sendto(client_socket, buffer, buffer_size, 0, (sockaddr*)&dest_addr, sin_size);
				break;
			}
			if (reakcja == '2')
			{
				std::cout << "<----wyslano komunikat o checi utrzymania polaczenia---->\n";
				odrzuc_zakonczenie();
				sendto(client_socket, buffer, buffer_size, 0, (sockaddr*)&dest_addr, sin_size);
				wyczysc();
			}
		}

		if (operacja == 3 && odpowiedz == 1)
		{
			std::cout << "<----zezwolono na rozlaczenie--->\n";
			std::cout << "<----trwa rozlaczanie--->\n";
			break;
		}

		if (operacja == 3 && odpowiedz == 4)
		{
			std::cout <<
				"<----rozmowca chce kontynuowac sesje---->\n[1] Ponow zadanie\n[2] Wymus zakonczenie\n[3] Wroc do rozmowy\n";
			int reakcja;
			reakcja = _getch();
			if (reakcja == '1')
			{
				std::cout << "<----ponawiam zadanie--->\n";
				zadaj_zakonczenia();
				sendto(client_socket, buffer, buffer_size, 0, (sockaddr*)&dest_addr, sin_size);
				wyczysc();
			}
			if (reakcja == '2')
			{
				std::cout << "<----wymuszono zakonczenie polaczenia---->\n";
				wymus_koniec();
				sendto(client_socket, buffer, buffer_size, 0, (sockaddr*)&dest_addr, sin_size);
			}
			if (reakcja == '3')
			{
				std::cout << "<----wracam do rozmowy---->\n";
				wyczysc();
			}
		}

		if (operacja == 7 && odpowiedz == 7)
		{
			std::cout << "<---rozmowca wymusil zakonczenie sesji--->\n";
			std::cout << "<----rozlaczono--->\n";
			break;
		}
	}
}
