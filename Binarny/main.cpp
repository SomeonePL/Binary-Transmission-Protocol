#include "server.h"
#include "client.h"


int main()
{
	std::cout << "Choose option:\n[1] Work as server\n[2] Work as client\n";

	int INPUT;
	do
	{
		INPUT = _getch();
		if (INPUT == '1')
		{
			std::cout << "Working as server\n";
			server s;
			s.UDP();
			break;
		}
		if (INPUT == '2')
		{
			std::cout << "Working as client\n";
			client c;
			c.UDP();
			break;
		}
	}
	while (INPUT != 1 || INPUT != 2);

	//std::cin.ignore(2);
}
