#include "SystemManager.h"
#include "Location.h"
#include "SharingLocation.h"
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
const string END_NODE_COLOR = "";

const string PATH_COLOR = "MAGENTA";
const float MAX_LAT = 41.20324;
const float MIN_LAT = 41.17303;
const float MAX_LON = -8.555458;
const float MIN_LON = -8.622682;

#define WINDOW_HEIGHT 2160
#define WINDOW_WIDTH 3840


SystemManager::SystemManager()
{
	gv = new GraphViewer(WINDOW_WIDTH, WINDOW_HEIGHT, false);
	gv->setBackground("backFEUP.png");
	gv->createWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

	gv->defineEdgeColor(EDGE_COLOR_DEFAULT);
	gv->defineVertexColor(VERTEX_COLOR_DEFAULT);
}

SystemManager::~SystemManager()
{
	delete gv;
}

void SystemManager::selectGraph()
{
	cout << "Welcome !!\n\n";
	cout << "Choose the Map you want:\n\n";
	cout << "1 - Baixa Porto - 666 Nodes, 1920 Edges.\n\n";
	cout << "2 - Zona Hospital Sao Joao / FEUP - X Nodes, Y Edges.\n\n";

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
				this->fileNames.sharingLocations = "sharingLocations.txt";
				repeat = false;
				break;
			}
			case 2:
			{
				this->fileNames.nodes = "nodesFEUP.txt";
				this->fileNames.edges = "edgesFEUP.txt";
				this->fileNames.names = "namesFEUP.txt";
				this->fileNames.sharingLocations = "sharingFEUP.txt";
				repeat = false;
				break;
			}
			case 3:
			{
				this->fileNames.nodes = "nodesPorto.txt";
				this->fileNames.edges = "edgesPorto.txt";
				this->fileNames.names = "nomesPorto.txt";
				this->fileNames.sharingLocations = "sharingPorto.txt";
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
	vector<SharingLoc> sharingLocations;

	begin = clock();
	loadSharingLocations(sharingLocations);
	end = clock();

	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to read Sharing Locations file: " << timeSpent << endl << endl;


	vector<pair<int, unsigned long long>> idsNodes;

	begin = clock();
	loadNodes(idsNodes, sharingLocations);
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

void SystemManager::loadSharingLocations(vector<SharingLoc>& sharingLocations)
{
	ifstream file(fileNames.sharingLocations);

	cout << "File: " << fileNames.sharingLocations << endl;

	if (!file.is_open())
	{
		cerr << "File not found!" << endl;
		exit(1);
	}

	unsigned long long id;
	char ign;
	int lotation, slots;

	while (!file.eof())
	{
		file >> id >> ign >> lotation >> ign >> slots;

		sharingLocations.push_back(SharingLoc(id, lotation, slots));
	}
	file.close();
}

void SystemManager::loadNodes(vector<pair<int, unsigned long long>> &idsNodes, const vector<SharingLoc> &sharingLocations)
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
			string isShrLoc;

			read >> id >> ign >> lat >> ign >> lon >> ign >> projx >> ign >> projy >> ign >> alt >> ign;
			getline(read, isShrLoc);

			gv->addNode(idInt, 2.5 * convertLongitudeToX(lon), -1 * convertLatitudeToY(lat) - 100);
			idsNodes.push_back(make_pair(idInt, id));
			if (isShrLoc == "true")
			{
				gv->setVertexColor(idInt, RED);
				for (auto sl : sharingLocations)
				{
					if (sl.id == id)
					{
						graph.addVertex(new SharingLocation(idInt, lat, lon, alt, sl.lotation, sl.slots));
						break;
					}
				}
			}
			else
			{
				graph.addVertex(new Location(idInt, lat, lon, alt));
			}

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
					origin = graph.findVertex(new Location(x.first));
					origemID = x.first;
					orig = true;
					if (des)
						break;
				}
				else if (x.second == dest)
				{
					destiny = graph.findVertex(new Location(x.first));
					destinoID = x.first;
					des = true;
					if (orig)
						break;
				}
			}
			double weight = calcWeight((origin->getInfo()), (destiny->getInfo()));

			for (auto x : edges)
			{
				if (x.id == id)
				{
					if (x.isBidirectional)
					{
						gv->addEdge(idIntEdge, origemID, destinoID, EdgeType::UNDIRECTED);
						graph.addEdge(origin->getInfo(), destiny->getInfo(), weight, idIntEdge, x.name);
						gv->setEdgeLabel(idIntEdge, x.name);
						idIntEdge++;					
						graph.addEdge(destiny->getInfo(), origin->getInfo(), weight, idIntEdge, x.name);					
					}
					else
					{
						gv->addEdge(idIntEdge, origemID, destinoID, EdgeType::DIRECTED);
						graph.addEdge(origin->getInfo(), destiny->getInfo(), weight, idIntEdge, x.name);
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

	int location, userChoice = 0;

	cout << "------------------------------" << endl;
	cout << "--------|RENT A BIKE|--------" << endl;
	cout << "------------------------------" << endl;
	cout << endl << "Tell me your location: ";


	cin >> location;


	while (userChoice != 1 && userChoice != 2)
	{
		cout << "Enter your preference" << endl;
		cout << "(1) Closest sharing location (no discount)" << endl;
		cout << "(2) Other sharing locations (discount)" << endl;

		cin >> userChoice;
		cin.ignore(1000, '\n');
		cin.clear();
	}

	Vertex* loc;

	try {
		loc = findLocation(location);
	}
	catch (LocationNotFound &e)
	{
		cerr << e.message();
		return false;
	}
	catch (...)
	{
		cout << "Unknown exception." << endl;
		return false;
	}

	switch (userChoice)
	{
	case 1:
		showClosestLocation(loc, location);
		break;
	case 2:
		showDiscountLocations(loc, location, true);
		break;
	default:
		break;
	}


	return true;
}

void SystemManager::showClosestLocation(Vertex* origin, int id)
{
	clock_t begin, end;
	begin = clock();

	Location* dest = NULL;
	bool success = graph.dijkstraShortestPath(origin->getInfo(), dest);
	end = clock();
	double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

	if (success)
	{
		vector<Vertex> path = graph.getPath(origin->getInfo(), dest);
		gv->setVertexColor(id, YELLOW);
		paintPath(path, true, 5, START_NODE_COLOR, END_NODE_COLOR, PATH_COLOR);
		cout << "Found closest location in: " << to_string(timeSpent) << " seconds!" << endl;
		cout << "Check the map to see the generated path!" << endl;
		system("pause");
		paintPath(path, false, 1);
	}
	else
	{
		cout << "Can't reach any sharing location from your location!" << endl;
		system("pause");
	}
}

void SystemManager::showDiscountLocations(Vertex* origin, int id, bool rent)
{
	vector<Vertex*> v = graph.discountLocations(rent, DISCOUNT_LOCATIONS);
	Vertex* dest = getDiscountChoice(v);

	clock_t begin, end;
	begin = clock();
	bool success = graph.dijkstraShortestPath(origin->getInfo(), dest);
	end = clock();
	double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

	if (success)
	{
		vector<Vertex> path = graph.getPath(origin->getInfo(), dest->getInfo());
		paintPath(path, true, 5, START_NODE_COLOR, END_NODE_COLOR, PATH_COLOR);
		cout << "Found requested location in: " << to_string(timeSpent) << " seconds!" << endl;
		system("pause");
		paintPath(path, false, 1);
	}
	else
	{
		cout << "Can't reach node " << to_string(dest->getInfo()->getID()) << " from your location!" << endl;
		system("pause");
	}
}

bool SystemManager::menuHasBike()
{
	Limpar_ecra();

	int location, userChoice = 0;

	cout << "------------------------------" << endl;
	cout << "--------|DELIVER A BIKE|--------" << endl;
	cout << "------------------------------" << endl;
	cout << endl << "Tell me your location: ";

	cin >> location;

	while (userChoice != 1 && userChoice != 2)
	{
		cout << "Enter your preference" << endl;
		cout << "(1) Closest sharing location (no discount)" << endl;
		cout << "(2) Other sharing locations (discount)" << endl;

		cin >> userChoice;
		cin.ignore(1000, '\n');
		cin.clear();
	}

	Vertex* loc;

	try {
		loc = findLocation(location);
	}
	catch (LocationNotFound &e)
	{
		cerr << e.message();
		return false;
	}
	catch (...)
	{
		cout << "Unknown exception." << endl;
		return false;
	}

	switch (userChoice)
	{
	case 1:
		showClosestLocation(loc, location);
		break;
	case 2:
		showDiscountLocations(loc, location, false);
		break;
	default:
		break;
	}

	return true;
}




/*
* Auxiliary function to find a vertex with a given stringName.
*/
Vertex* SystemManager::findLocation(const int ID) const {
	for (auto v : graph.getVertexSet())
		if (v->getInfo()->getID() == ID)
			return v;

	throw LocationNotFound(ID);

}

int SystemManager::convertLongitudeToX(float longitude) {
	return floor(((longitude - MIN_LON) * (WINDOW_HEIGHT)) / (MAX_LON - MIN_LON));
}

int SystemManager::convertLatitudeToY(float latitude) {
	return floor(((latitude - MIN_LAT) * (WINDOW_WIDTH)) / (MAX_LAT - MIN_LAT));
}

void SystemManager::paintPath(vector<Vertex> path, bool def, int edgeThickness, string startNodeColor, string endNodeColor, string edgeColor)
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

	if (def)
	{
		gv->setVertexColor(path.begin()->getInfo()->getID(), startNodeColor);
		gv->setVertexColor(path.back().getInfo()->getID(), endNodeColor);
	}
	else
	{
		gv->setVertexColor(path.begin()->getInfo()->getID(), path.begin()->getInfo()->getColor());
		gv->setVertexColor(path.back().getInfo()->getID(), path.back().getInfo()->getColor());
	}
}

Vertex* SystemManager::getDiscountChoice(const vector<Vertex*> &v) const
{
	cout << "If you choose one of the following locations, you can get 50 euros in discount!" << endl;
	cout << "Here are they're IDs: " << endl;

	int userChoice;

	for (int i = 0; i < v.size(); i++)
		gv->setVertexColor(v[i]->getInfo()->getID(), "PINK");

	auto exists = find_if(v.begin(), v.end(), [userChoice](Vertex* vertex) {
		return vertex->getInfo()->getID() == userChoice;
	});

	while (exists == v.end())
	{
		for (int i = 0; i < v.size(); i++)
			cout << "Sharing Location: " << v[i]->getInfo()->getID() << endl;

		cout << "Enter your preference: ";
		cin >> userChoice;

		exists = find_if(v.begin(), v.end(), [userChoice](Vertex* vertex) {
			return vertex->getInfo()->getID() == userChoice;
		});
	}

	return findLocation(userChoice);
}
