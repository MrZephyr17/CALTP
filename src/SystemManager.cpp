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

#define WINDOW_HEIGHT 2160
#define WINDOW_WIDTH 3840

long highest_id;

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

	vector<pair<int, unsigned long long>> idsNodes;
	vector<pair<int, unsigned long long>> idsEdges;

	begin = clock();
		loadNodes(idsNodes);
	end = clock();
	
	
	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to read Nodes file: " << timeSpent << endl << endl;

	begin = clock();
		vector<EdgeInfo> edges = loadEdges(idsEdges, idsNodes);
	end = clock();
	
	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to read Edges file: " << timeSpent << endl << endl;

	begin = clock();
	//	loadNames(edges, idsEdges);
	end = clock();

	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to read Names file: " << timeSpent << endl << endl;

	//gv->rearrange();
}

vector<EdgeInfo> SystemManager::loadEdges(vector<pair<int, unsigned long long>> &idsEdge, vector<pair<int, unsigned long long>> &idsNode)
{
	ifstream read(fileNames.edges);
	vector<EdgeInfo> edges;

	cout << "File: " << fileNames.edges << endl;
	int idIntEdge = 0;
	unsigned long long id = -1, ori, dest;
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
			idIntEdge++;
			read >> id >> ign >> ori >> ign >> dest >> ign ;


			Vertex* origin;
			Vertex* destiny;
			bool orig = false, des = false;
			int origemID = 0, destinoID = 0;
			for (auto x : idsNode)
			{
				if (x.second == ori)
				{
					origin = graph.findVertex(Location(x.first));
					origemID = x.first;
					orig = true;
					if (des)
						break;
				}
				else if (x.second == dest)
				{
					destiny = graph.findVertex(Location(x.first));
					destinoID = x.first;
					des = true;
					if (orig)
						break;
				}
			}
			
			//double weight = calcWeight(&(origin->getInfo()), &(destiny->getInfo()));
			
			idsEdge.push_back(make_pair(idIntEdge, id));
			edges.push_back(EdgeInfo(idIntEdge, &origin->getInfo(), &destiny->getInfo()));
			gv->addEdge(idIntEdge, origemID, destinoID, EdgeType::DIRECTED);
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

void SystemManager::loadNodes(vector<pair<int, unsigned long long>> &idsNodes)
{
	ifstream read(fileNames.nodes);
	
	int idInt = 0;

	cout << "File: " << fileNames.nodes << endl;

	if (!read.is_open())
	{
		cerr << "File not found!" << endl;
		exit(1);
	}
	else
	{
		while (!read.eof())
		{
			idInt++;
			unsigned long long id;


			double lat, lon, projx, projy, alt = 1;
			char ign;
			string junk;
			
			read >> id >> ign >> lat >> ign >> lon >> ign >> projx >> ign >> projy >> ign >> alt;
			getline(read, junk);

			graph.addVertex(Location(idInt, lat, lon, alt));

			idsNodes.push_back(make_pair(idInt, id));

			gv->addNode(idInt);
			
			//gv->addNode(id,projx, projy);

		}
		read.close();
	}
}

void SystemManager::loadNames(vector<EdgeInfo> edges, vector<pair<int, unsigned long long>> &idsEdges)
{
	ifstream read(fileNames.names);
	cout << "File: " << fileNames.names << endl;

	if (!read.is_open())
	{
		cerr << "File not found!" << endl;
		exit(1);
	}
	else
	{
		while (!read.eof())
		{
			unsigned long long id;
			string name, isBidirectional;
			char ign;

			read >> id >> ign;

			getline(read, name, ';');
			getline(read, isBidirectional);

			
			for (auto edg : idsEdges)
			{
				if (edg.second == id)
				{
					for (auto it : edges)
					{
						if (it.id == edg.first)
						{
							graph.addEdge(it.origin, it.dest, calcWeight(it.origin, it.dest), edg.first, name);
							gv->addEdge(edg.first, it.origin->getID(), it.dest->getID(), EdgeType::DIRECTED);
							break;
							/*if (isBidirectional == "True")
							{
							graph.addEdge(it.dest, it.origin, calcWeight(it.dest, it.origin), ++highest_id, name);
							gv->addEdge(highest_id, it.dest->getID(), it.origin->getID(), EdgeType::DIRECTED);
							}*/
						}
					}
				}
			}
		}
		//gv->rearrange();
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
