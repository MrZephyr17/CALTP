#include "SystemManager.h"
#include "Location.h"
#include "Exceptions.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <map>

using namespace std;

// CONSTANTS
#define EDGE_COLOR_DEFAULT NONE
#define VERTEX_COLOR_DEFAULT NONE

#define WINDOW_HEIGHT 1
#define WINDOW_WIDTH 2

long highest_id;

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
				break;
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
	clock_t begin, end;
	double timeSpent;

	begin = clock();
		loadVertexes();
	end = clock();
	
	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to read Nodes file: " << timeSpent << endl << endl;

	begin = clock();
		vector<EdgeInfo> edges = loadEdges();
	end = clock();
	
	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to read Edges file: " << timeSpent << endl << endl;

	begin = clock();
		loadNames(edges);
	end = clock();

	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to read Names file: " << timeSpent << endl << endl;
}

vector<EdgeInfo> SystemManager::loadEdges()
{
	ifstream read(fileNames.edges);
	vector<EdgeInfo> edges;

	cout << fileNames.edges << endl;

	int id = -1, ori, dest;
	char ign;

	if (!read.is_open())
	{
		cerr << "File not found!" << endl;
		exit(1);
	}
	else
	{
		while (!read.eof())
		{
			read >> id >> ign >> ori >> ign >> dest >> ign;

			//cout << "\nid = " << id;
			//cout << "\nori = " << ori;
			//cout << "\ndest = " << dest;

			//system("pause");

			Vertex* origin = graph.findVertex(Location(ori));
			Vertex* destiny = graph.findVertex(Location(dest));

			double weight;

			weight = calcWeight(origin->getInfo(), destiny->getInfo());


			edges.push_back(EdgeInfo(id, origin->getInfo(), destiny->getInfo()));
		}

		read.close();
	}

	if (id == -1)
	{
		cerr << "Graph with 0 elements." << endl;
		exit(2);
	}
	highest_id = id;

	return edges;
}

void SystemManager::loadVertexes()
{
	ifstream read(fileNames.nodes);

	cout << fileNames.nodes << endl;

	if (!read.is_open())
	{
		cerr << "File not found!" << endl;
		exit(1);
	}
	else
	{
		while (!read.eof())
		{
			int id;
			double lat, lon, projx, projy, alt = 1;
			char ign;

			read >> id >> ign >> lat >> ign >> lon;		// >> projx >> ign >> projy >> ign >> alt;
			
			//cout << "id = " << id << endl;
			//cout << "lat = " << lat << endl;
			//cout << "lon = " << lon << endl;
			//cout << "projx = " << projx << endl;
			//cout << "projy = " << projy << endl;
			//cout << "alt = " << alt << endl;
			//system("pause");

			graph.addVertex(Location(id, lat, lon, alt));
			//gv->addNode(id, projx, projy);
		}
		read.close();
	}
}

void SystemManager::loadNames(vector<EdgeInfo> edges)
{
	ifstream read(fileNames.names);
	cout << fileNames.names << endl;

	if (!read.is_open())
	{
		cerr << "File not found!" << endl;
		exit(1);
	}
	else
	{
		while (!read.eof())
		{
			int id;
			string name, isBidirectional, junk;
			char ign;

			read >> id >> ign;

			getline(read, name, ';');
			getline(read, isBidirectional, ';');
			getline(read, junk);

			auto it = find_if(edges.begin(), edges.end(), [id](const EdgeInfo& e) {return e.id == id; });
			graph.addEdge(it->origin, it->dest, calcWeight(it->origin, it->dest), id, name);
			//gv->addEdge(id, it->origin.getID(), it->dest.getID(), EdgeType::DIRECTED);
			
			//cout << "\nid = " << id;
			//cout << "\nign = " << ign;
			//cout << "\nname = " << name;
			//cout << "\nisBidirectional = " << isBidirectional;
			//cout << "\njunk = " << junk;
			//system("pause");

			if (isBidirectional == "True")
			{
				graph.addEdge(it->dest, it->origin, calcWeight(it->dest, it->origin), ++highest_id, name);
				//gv->addEdge(highest_id, it->dest.getID(), it->origin.getID(), EdgeType::DIRECTED);
			}
		}

		read.close();
	}
}

bool SystemManager::Menu()
{
	Limpar_ecra();

	selectGraph();

	loadFiles();

	system("pause");

	mainMenu();

	return true;
}

bool SystemManager::mainMenu()
{
	int option;

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
				break;
			}
			case 2:
			{
				menuHasBike();
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

	cout << "------------------------------" << endl;
	cout << "--------|RENT A BIKE|--------" << endl;
	cout << "------------------------------" << endl;
	cout << endl << "Tell me your location: ";


	getline(cin, location);

	try
	{
		Vertex* loc = findLocation(location);
		Location  dest = graph.dijkstraShortestPath(loc->getInfo());
		vector<Vertex> path = graph.getPath(loc->getInfo(), dest);

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
	
	return true;
}


bool SystemManager::menuHasBike()
{
	Limpar_ecra();
	
	string location;

	cout << "------------------------------" << endl;
	cout << "--------|DELIVER A BIKE|--------" << endl;
	cout << "------------------------------" << endl;
	cout << endl << "Tell me your location: ";

	getline(cin, location);

	try
	{
		 Vertex* loc = findLocation(location);
		 Location  dest = graph.dijkstraShortestPath(loc->getInfo());
		 vector<Vertex> path = graph.getPath(loc->getInfo(), dest);
		 
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
	
	return false;
}


/*
* Auxiliary function to find a vertex with a given stringName.
*/
Vertex* SystemManager::findLocation(const string name) const {
	for (auto v : graph.getVertexSet())
		if (v->getInfo().getName() == name)
			return v;
	throw LocationNotFound(name);

}
