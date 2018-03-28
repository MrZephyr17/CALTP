#include "SystemManager.h"
#include "Location.h"
#include "Exceptions.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

// CONSTANTS
#define EDGE_COLOR_DEFAULT NONE
#define VERTEX_COLOR_DEFAULT NONE

#define WINDOW_HEIGHT 1
#define WINDOW_WIDTH 2

SystemManager::SystemManager()
{
	gv = new GraphViewer(WINDOW_WIDTH, WINDOW_HEIGHT, false);
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
				this->fileNames.names = "namesBaixa.txt";

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
	loadVertexes();

	vector<EdgeInfo> edges = loadEdges();

	loadNames(edges);
}

vector<EdgeInfo> SystemManager::loadEdges()
{
	ifstream read(fileNames.edges);
	vector<EdgeInfo> edges;

	if (!read.is_open())
	{
		cerr << "File not found!" << endl;
	}

	while (!read.eof())
	{
		int id, ori, dest;
		char ign;

		read >> id >> ign >> ori >> ign >> dest;

		Vertex<Location>* origin = graph.findVertex(Location(ori));
		Vertex<Location>* destiny = graph.findVertex(Location(dest));

		double weight;

		weight = calcWeight(origin->getInfo(), destiny->getInfo());


		edges.push_back(EdgeInfo(id, origin->getInfo(), destiny->getInfo()));
	}

	return edges;
}

void SystemManager::loadVertexes()
{
	ifstream read(fileNames.nodes);

	if (!read.is_open())
	{
		cerr << "File not found!" << endl;
	}

	while (!read.eof())
	{
		int id;
		double lat, lon, projx, projy, alt;
		char ign;

		read >> id >> ign >> lat >> ign >> lon >> projx >> ign >> projy >> ign >> alt;
		graph.addVertex(Location(id, lat, lon, alt));
	}

	read.close();
}

void SystemManager::loadNames(vector<EdgeInfo> edges)
{
	ifstream read(fileNames.names);

	if (!read.is_open())
	{
		cerr << "File not found!" << endl;
	}

	while (!read.eof())
	{
		int id;
		string name, isBidirectional;
		bool bidirectional;
		char ign;

		read >> id >> ign;

		getline(read, name, ';');
		getline(read, isBidirectional, ';');

		bidirectional = isBidirectional == "False" ? false : true;

		auto it = find_if(edges.begin(), edges.end(), [id](const EdgeInfo& e) {return e.id == id; });
		graph.addEdge(it->origin, it->dest, calcWeight(it->origin, it->dest), id, name);

		if (bidirectional)
			graph.addEdge(it->dest, it->origin, calcWeight(it->dest, it->origin), id, name);
	}

	read.close();
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
		findLocation(location);
	}
	catch (LocationNotFound &e)
	{
		cerr << e.message();
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
		findLocation(location);
	}
	catch (LocationNotFound &e)
	{
		cerr << e.message();
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
