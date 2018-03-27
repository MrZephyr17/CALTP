#include "SystemManager.h"
#include "Location.h"
#include "Exceptions.h"
#include <iostream>
#include <string>

using namespace std;

// CONSTANTS
#define EDGE_COLOR_DEFAULT NONE
#define VERTEX_COLOR_DEFAULT NONE

SystemManager::SystemManager()
{
	graph = new Graph<Location>;
}

SystemManager::~SystemManager()
{
}


void SystemManager::selectGraph()
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


void SystemManager::loadFiles()
{
	/*try 
	{
		loadEdges();
	}
	catch ()
	{ 
	}
	 
	try 
	{
		loadVertexes();
	} 
	catch ()
	{

	}

	try
	{
		loadNames();
	}
	catch ()
	{

	}*/
}

void SystemManager::loadEdges()
{

}

void SystemManager::loadVertexes()
{

}

void SystemManager::loadNames()
{

}


bool SystemManager::Menu()
{
	Limpar_ecra();

	selectGraph();

	loadFiles();

	mainMenu();

	return true;
}


bool SystemManager::mainMenu()
{
	int option;
	bool repeat = true;

	while (true)
	{
		Limpar_ecra();

		// main menu
		cout << "------------------------------" << endl;
		cout << "------------|MENU|------------" << endl;
		cout << "------------------------------" << endl;
		cout << "1 - Rent a bike" << endl;
		cout << "2 - I already have one" << endl;
		cout << "3 - Select new graph" << endl;
		cout << "4 - Exit" << endl;

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
				menuRent();
				//repeat = false;
				break;
			}
			case 2:
			{
				menuHasBike();
				//repeat = false;
				break;
			}
			case 3:
			{
				return true;
			}
			case 4:
			{
				exit(0);
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


bool SystemManager::menuRent()
{
	Limpar_ecra();

	string location;
	bool repeat = true;

	cout << "------------------------------" << endl;
	cout << "--------|RENT A BIKE|--------" << endl;
	cout << "------------------------------" << endl;
	cout << endl << "Tell me your location: ";


	getline(cin, location);

	try
	{
		//nearestRentLocation(location);
	}
	catch (LocationNotFound &e)
	{
		cout << "Location Not Found: The location: " << e.location << " already exists." << endl;
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


bool SystemManager::menuHasBike()
{
	Limpar_ecra();

	string location;
	bool repeat = true;

	cout << "------------------------------" << endl;
	cout << "--------|DELIVER A BIKE|--------" << endl;
	cout << "------------------------------" << endl;
	cout << endl << "Tell me your location: ";

	getline(cin, location);

	try
	{
		//nearestDeliveryLocation(location);
	}
	catch (LocationNotFound &e)
	{
		cout << "Location Not Found: The location: " << e.location << " already exists." << endl;
		system("pause");
	}
	catch (...)
	{
		cout << "Unknown exception." << endl;
		system("pause");
	}
	repeat = false;

	return false;
}


/*
* Auxiliary function to find a vertex with a given stringName.
*/
Vertex<Location> * SystemManager::findLocation(const string name) const {
	for (auto v : graph.getVertexSet())
		if (v->getInfo().getName() == name)
			return v;
	throw LocationNotFound(name);	
}