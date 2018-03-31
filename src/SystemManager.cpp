#include "SystemManager.h"
#include "Location.h"
#include "Exceptions.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include <map>

using namespace std;

// CONSTANTS
#define EDGE_COLOR_DEFAULT NONE
#define VERTEX_COLOR_DEFAULT NONE

#define WINDOW_HEIGHT 2160
#define WINDOW_WIDTH 3840

unsigned long long highest_id;

SystemManager::SystemManager()
{
	gv = new GraphViewer(WINDOW_WIDTH, WINDOW_HEIGHT, true);
	gv->setBackground("back.jpg");
	gv->createWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

	gv->defineEdgeColor("black");
	gv->defineVertexColor("blue");

}

SystemManager::~SystemManager()
{
}

void SystemManager::selectGraph()
{
	cout << "Welcome !!\n\n";
	cout << "Choose the Map you want:\n\n";
	cout << "Baixa Porto - 666 Nodes, 1920 Edges. Choose 1!\n\n";

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
				this->fileNames.nodes = "nodes.txt";
				this->fileNames.edges = "edges.txt";
				this->fileNames.names = "names.txt";

				repeat = false;
				break;
			}
			case 2:
			{
				this->fileNames.nodes = "s.txt";
				this->fileNames.edges = "s.txt";
				this->fileNames.names = "s.txt";

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

	ifstream readNodes, readEdges, readNames;

	begin = clock();

	readNodes.open(fileNames.nodes);

	cout << "File: " << fileNames.nodes << endl;

	if (!readNodes.is_open())
	{
		cerr << "File not found!" << endl;
		exit(1);
	}
	else
	{
		string node, junk;

		while (!readNodes.eof())
		{
			node = "";
			getline(readNodes, node);
			loadNodes(node);
			getline(readNodes, junk);
		}

		readNodes.close();
	}

	end = clock();

	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to read Nodes file: " << timeSpent << endl << endl;

	begin = clock();

	vector<EdgeInfo> edges;

	readEdges.open(fileNames.edges);

	cout << "File: " << fileNames.edges << endl;

	if (!readEdges.is_open())
	{
		cerr << "File not found!" << endl;
		exit(1);
	}
	else
	{
		string edge;

		while (!readEdges.eof())
		{
			edge = "";
			getline(readNodes, edge);
			loadEdges(edges, edge);
		}

		readEdges.close();
	}

	end = clock();

	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to read Edges file: " << timeSpent << endl << endl;

	begin = clock();
	
	readNames.open(fileNames.names);

	cout << "File: " << fileNames.names << endl;

	if (!readNames.is_open())
	{
		cerr << "File not found!" << endl;
		exit(1);
	}
	else
	{
		string name;

		while (!readNames.eof())
		{
			name = "";
			getline(readNames, name);
			loadEdges(edges, name);
		}

		readNames.close();
	}
	end = clock();

	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to read Names file: " << timeSpent << endl << endl;
}

void SystemManager::loadEdges(vector<EdgeInfo> &edges, string edge)
{
	stringstream edge_stream(edge);
	unsigned long long id = -1, ori, dest;
	char ign;

	edge_stream >> id >> ign >> ori >> ign >> dest >> ign;

	Vertex* origin = graph.findVertex(Location(ori));
	Vertex* destiny = graph.findVertex(Location(dest));

	//double weight = calcWeight(&(origin->getInfo()), &(destiny->getInfo()));

	edges.push_back(EdgeInfo(id, &origin->getInfo(), &destiny->getInfo()));

	highest_id = id;
}

void SystemManager::loadNodes(string node)
{
	stringstream node_stream(node);
	unsigned long long id;
	double lat, lon, projx, projy, alt = 1;
	char ign;
	string junk;

	node_stream >> id >> ign >> lat >> ign >> lon >> ign >> projx >> ign >> projy >> ign >> alt;

	graph.addVertex(Location(id, lat, lon, alt));
	gv->addNode(id);
	//gv->addNode(id,projx, projy);
}

void SystemManager::loadNames(vector<EdgeInfo> edges, string name)
{
	stringstream names_stream(name);

	unsigned long long id;
	string nome, isBidirectional;
	char ign;

	 names_stream>> id >> ign;

	getline(names_stream, nome, ';');
	getline(names_stream, isBidirectional);

	for (auto it : edges)
	{
		if (it.id == id)
		{
			graph.addEdge(it.origin, it.dest, calcWeight(it.origin, it.dest), id, nome);
			gv->addEdge(id, it.origin->getID(), it.dest->getID(), EdgeType::DIRECTED);
		}
	}

	/*	if (isBidirectional == "True")
		{
			graph.addEdge(it->dest, it->origin, calcWeight(it->dest, it->origin), ++highest_id, name);
			gv->addEdge(highest_id, it->dest->getID(), it->origin->getID(), EdgeType::DIRECTED);
		}*/
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
