#include "SystemManager.h"
#include "Location.h"
#include "Exceptions.h"
#include <iostream>
#include <string>

using namespace std;

// CONSTANTS
#define EDGE_COLOR_DEFAULT NONE
#define VERTEX_COLOR_DEFAULT NONE

template<class T>
SystemManager<T>::SystemManager()
{
}

template<class T>
SystemManager<T>::~SystemManager()
{
}

template<class T>
void SystemManager<T>::selectGraph()
{
	cout << "		Welcome !!\n\n";
	cout << " Choose the Map you want:\n\n";
	cout << "    Baixa Porto	- 666 Nodes, 1920 Edges. Choose 1!\n\n";

	bool repeat = true;
	int option;

	while (repeat)
	{
		cin >> option;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Insert a valid option." << endl;
			continue;
		}
		else
		{
			cin.ignore(1000, '\n');

			switch (option)
			{
			case 1:
			{
				this->fileNames.nodes = "nodesBaixa.txt";
				this->fileNames.edges = "edgesBaixa.txt";
				this->fileNames.lines = "linesBaixa.txt";

				repeat = false;
			}
			default:
				cout << "Insert a valid menu option." << endl;
				break;
			}
		}
	}
}

template<class T>
void SystemManager<T>::loadFiles()
{
	// load files
}

template<class T>
bool SystemManager<T>::Menu()
{
	selectGraph();

	loadFiles();

	Limpar_ecra();

	mainMenu();

	return false;
}

template<class T>
bool SystemManager<T>::mainMenu()
{
	int option;
	bool repeat = true;

	// main menu
	cout << "------------------------------" << endl;
	cout << "------------|MENU|------------" << endl;
	cout << "------------------------------" << endl;
	cout << "1 - Rent a bike" << endl;
	cout << "2 - I already have one" << endl;
	cout << "3 - Exit" << endl;

	while (repeat)
	{
		cin >> option;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Insert a valid menu option." << endl;
			continue;
		}
		else
		{
			cin.ignore(1000, '\n');
			switch (option)
			{
			case 1:
			{
				Limpar_ecra();
				menuRent();
				Limpar_ecra();
				repeat = false;
				return true;
			}
			case 2:
			{
				Limpar_ecra();
				menuHasBike();
				Limpar_ecra();
				repeat = false;
				return true;
			}
			case 3:
			{
				return false;
			}
			default:
			{
				cout << "Insert a valid menu option." << endl;
			}
			}
		}
	}
	return true;
}

template<class T>
bool SystemManager<T>::menuRent()
{
	string location;
	bool repeat = true;

	cout << "------------------------------" << endl;
	cout << "--------|RENT A BIKE|--------" << endl;
	cout << "------------------------------" << endl;
	cout << endl << "Tell me your location: ";

	cin << location << endl;
	
	try
	{
		//nearestRentLocation(location);
	}
	catch (LocationNotFound &e)
	{
		cout << "Location Not Found: The location: " << e.getLocation() << " already exists." << endl;
		system("pause");
	}
	catch (...)
	{
		cout << "Unknown exception." << endl;
		system("pause");
	}
	repeat = false;

	return true;
}

template<class T>
bool SystemManager<T>::menuHasBike()
{
	string location;
	bool repeat = true;

	cout << "------------------------------" << endl;
	cout << "--------|DELIVER A BIKE|--------" << endl;
	cout << "------------------------------" << endl;
	cout << endl << "Tell me your location: ";

	cin << location << endl;

	try
	{
		//nearestDeliveryLocation(location);
	}
	catch (LocationNotFound &e)
	{
		cout << "Location Not Found: The location: " << e.getLocation() << " already exists." << endl;
		system("pause");
	}
	catch (...)
	{
		cout << "Unknown exception." << endl;
		system("pause");
	}
	repeat = false;

	return true;
}