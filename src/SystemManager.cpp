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
#define EDGE_COLOR_DEFAULT BLACK
#define VERTEX_COLOR_DEFAULT BLUE

const string START_NODE_COLOR = "YELLOW";
const string END_NODE_COLOR = "GREEN";

const string PATH_COLOR = "MAGENTA";
const float MAX_LAT = 41.16246;
const float MIN_LAT = 41.14566;
const float MAX_LON = -8.598584;
const float MIN_LON = -8.618032;

#define WINDOW_HEIGHT 2160
#define WINDOW_WIDTH 3840


SystemManager::SystemManager()
{
	gv = new GraphViewer(WINDOW_WIDTH, WINDOW_HEIGHT, false);
	gv->setBackground("back.jpg");
	gv->createWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

	gv->defineEdgeColor(EDGE_COLOR_DEFAULT);
	gv->defineVertexColor(VERTEX_COLOR_DEFAULT);
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

	begin = clock();
		loadNodes(idsNodes);
	end = clock();


	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to read Nodes file: " << timeSpent << endl << endl;


	begin = clock();
		vector<EdgeName> edges = loadNames();
	end = clock();

	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to read Names file: " << timeSpent << endl << endl;

	begin = clock();
		loadEdges(edges, idsNodes);
	end = clock();

	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to read Edges file: " << timeSpent << endl << endl;

	gv->rearrange();
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
			gv->addNode(idInt, 3 * convertLatitudeToY(lat), 3 * convertLongitudeToX(lon));
			
			gv->setVertexLabel(idInt, to_string(idInt));

		}
		read.close();
	}
}

vector<EdgeName> SystemManager::loadNames()
{
	ifstream read(fileNames.names);
	cout << "File: " << fileNames.names << endl;

	vector<EdgeName> edges;

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

			edges.push_back(EdgeName(id, name, isBidirectional == "True" ? true : false));
		}
		read.close();
	}

	return edges;
}

void SystemManager::loadEdges(vector<EdgeName> &edges, vector<pair<int, unsigned long long>> &idsNode)
{
	ifstream read(fileNames.edges);

	cout << "File: " << fileNames.edges << endl;

	unsigned long long id = -1, ori, dest;
	char ign;

	if (!read.is_open())
	{
		cerr << "File not found!" << endl;
		exit(1);
	}
	else
	{
		int idIntEdge = 0;
		while (!read.eof())
		{
			idIntEdge++;
			read >> id >> ign >> ori >> ign >> dest >> ign;


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

			double weight = calcWeight(&(origin->getInfo()), &(destiny->getInfo()));

			for (auto x : edges)
			{
				if (x.id == id)
				{
					if (x.isBidirectional)
					{
						gv->addEdge(idIntEdge, origemID, destinoID, EdgeType::DIRECTED);
						graph.addEdge(&origin->getInfo(), &destiny->getInfo(), weight, idIntEdge, x.name);
						gv->setEdgeLabel(idIntEdge, x.name);
						idIntEdge++;
						gv->addEdge(idIntEdge, destinoID, origemID, EdgeType::DIRECTED);
						graph.addEdge(&destiny->getInfo(), &origin->getInfo(), weight, idIntEdge, x.name);
						gv->setEdgeLabel(idIntEdge, x.name);
					}
					else
					{
						gv->addEdge(idIntEdge, origemID, destinoID, EdgeType::DIRECTED);
						graph.addEdge(&origin->getInfo(), &destiny->getInfo(), weight, idIntEdge, x.name);
						gv->setEdgeLabel(idIntEdge, x.name);
					}
					break;
				}
			}
		}

		read.close();
	}

	if (id == -1)
	{
		cerr << "Graph with 0 elements." << endl;
		exit(2);
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

	int location;

	cout << "------------------------------" << endl;
	cout << "--------|RENT A BIKE|--------" << endl;
	cout << "------------------------------" << endl;
	cout << endl << "Tell me your location: ";


	cin >> location;

	try
	{
		Vertex* loc = findLocation(location);
		Location  dest = graph.dijkstraShortestPath(loc->getInfo());
		vector<Vertex> path = graph.getPath(loc->getInfo(), dest);
		gv->setVertexColor(location, YELLOW);
		paintPath(path, START_NODE_COLOR, END_NODE_COLOR, PATH_COLOR, 5);
		system("pause");
		paintPath(path, VERTEX_COLOR_DEFAULT, VERTEX_COLOR_DEFAULT, EDGE_COLOR_DEFAULT, 1);
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

	int location;

	cout << "------------------------------" << endl;
	cout << "--------|DELIVER A BIKE|--------" << endl;
	cout << "------------------------------" << endl;
	cout << endl << "Tell me your location: ";

	cin >> location;

	try
	{
		Vertex* loc = findLocation(location);
		Location  dest = graph.dijkstraShortestPath(loc->getInfo());
		vector<Vertex> path = graph.getPath(loc->getInfo(), dest);
		paintPath(path, START_NODE_COLOR, END_NODE_COLOR, PATH_COLOR, 5);
		system("pause");
		paintPath(path, VERTEX_COLOR_DEFAULT, VERTEX_COLOR_DEFAULT, EDGE_COLOR_DEFAULT, 1);

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
Vertex* SystemManager::findLocation(const int ID) const {
	for (auto v : graph.getVertexSet())
		if (v->getInfo().getID() == ID)
			return v;

	throw LocationNotFound(ID);

}

int SystemManager::convertLongitudeToX(float longitude) {
	return floor(((longitude - MIN_LON) * (WINDOW_HEIGHT)) / (MAX_LON - MIN_LON));
}

int SystemManager::convertLatitudeToY(float latitude) {
	return floor(((latitude - MIN_LAT) * (WINDOW_WIDTH)) / (MAX_LAT - MIN_LAT));
}

void SystemManager::paintPath(vector<Vertex> path, string startNodeColor, string endNodeColor, string edgeColor, int edgeThickness)
{
	if (path.size() < 1) {
		cout << "Path not found.\nAre you sure there is a connection?\n";
		return;
	}
	for (int i = 0; i < path.size() - 1; i++)
	{
		Edge edge = graph.findEdge(path[i].getInfo(), path[i + 1].getInfo());

		if (edge.getID() == -1)
		{
			cout << "Deu merda no Djikstra / Path nao e possivel!!" << endl;
			return;
		}

		gv->setEdgeColor(edge.getID(), edgeColor);
		gv->setEdgeThickness(edge.getID(), edgeThickness);
	}

	gv->setVertexColor(path.begin()->getInfo().getID(), startNodeColor);
	gv->setVertexColor(path.back().getInfo().getID(), endNodeColor);
}
