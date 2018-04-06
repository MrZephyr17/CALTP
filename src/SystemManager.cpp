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
#define START_NODE_COLOR YELLOW
#define END_NODE_COLOR WHITE
#define PATH_COLOR MAGENTA
#define TAB "      "

const float MAX_LAT = 41.20324;
const float MIN_LAT = 41.17303;
const float MAX_LON = -8.555458;
const float MIN_LON = -8.622682;

#define WINDOW_HEIGHT 2160
#define WINDOW_WIDTH 3840

SystemManager::SystemManager()
{
	gv = new GraphViewer(WINDOW_WIDTH, WINDOW_HEIGHT, false);
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
	cout << TAB << "1 - Baixa Porto - 763 Nodes, 799 Edges, 20 Sharing Locations.\n\n";
	cout << TAB << "2 - Zona Hospital Sao Joao / FEUP - 969 Nodes, 1051 Edges, 20 Sharing Locations.\n\n";
	cout << TAB << "3 - Porto Paranhos - 8670 Nodes, 9278 Edges, 50 Sharing Locations.\n\n";
	cout << "Test files:\n\n";
	cout << TAB << "4 - Non Connective Graph.\n\n";
	cout << TAB << "5 - Connective Graph.\n\n";
	cout << TAB << "6 - Biconnective Graph.\n\n";

	int userChoice = verifyInput(1, 6);
	graph = Graph();
	if (gv != nullptr)
	{
		gv->closeWindow();
		delete gv;

		gv = new GraphViewer(WINDOW_WIDTH, WINDOW_HEIGHT, false);
		gv->createWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

		gv->defineEdgeColor(EDGE_COLOR_DEFAULT);
		gv->defineVertexColor(VERTEX_COLOR_DEFAULT);
	}

	switch (userChoice)
	{
	case 1:
		this->fileNames.nodes = "AHnodes.txt";
		this->fileNames.edges = "AHedges.txt";
		this->fileNames.names = "AHnames.txt";
		this->fileNames.sharingLocations = "AHsharingLocations.txt";
		break;
	case 2:
		this->fileNames.nodes = "FeupNodes.txt";
		this->fileNames.edges = "FeupEdges.txt";
		this->fileNames.names = "FeupNames.txt";
		this->fileNames.sharingLocations = "FeupSharing.txt";
		break;
	case 3:
		this->fileNames.nodes = "PortoNodes.txt";
		this->fileNames.edges = "PortoEdges.txt";
		this->fileNames.names = "PortoNomes.txt";
		this->fileNames.sharingLocations = "PortoSharing.txt";
		break; // ADD OTHERS CASES
	case 4:
		this->fileNames.nodes = "//Conetividade//nodesNaoConexo.txt";
		this->fileNames.edges = "//Conetividade//edgesNaoConexo.txt";
		this->fileNames.names = "//Conetividade//nomesNaoConexo.txt";
		this->fileNames.sharingLocations = "//Conetividade//sharingLocationsEmpty.txt";
		break;
	case 5:
		this->fileNames.nodes = "nodesConectividade.txt";
		this->fileNames.edges = "edgesConectividade.txt";
		this->fileNames.names = "nomesConectividade.txt";
		this->fileNames.sharingLocations = "sharingLocationsEmpty.txt";
		break;
	case 6:
		this->fileNames.nodes = "//Conetividade//nodesBiconectividade.txt";
		this->fileNames.edges = "//Conetividade//edgesBiconectividade.txt";
		this->fileNames.names = "//Conetividade//nomesBiconectividade.txt";
		this->fileNames.sharingLocations = "//Conetividade//sharingLocationsEmpty.txt";
		break;
	default:
		break;
	}
}

vector<pair<int, unsigned long long>> SystemManager::loadFiles()
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

	return idsNodes;
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
			gv->setVertexLabel(idInt, to_string(idInt));

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

bool SystemManager::isConnected()
{
	Limpar_ecra();
	double timeSpent;
	clock_t begin, end;
	begin = clock();

	cout << "Checking graph connectivity...\n";

	vector<Vertex *> vertexes = graph.getVertexSet();

	for (unsigned int id = 0; id < vertexes.size(); id++)
	{
		int counter = 0;

		for (Vertex * v : vertexes)
		{
			v->getInfo()->setVisited(false);
		}

		isConnectedAux(vertexes.at(134),counter);
		gv->setVertexColor(id + 1, MAGENTA);
		
		if (counter != vertexes.size())
		{
			end = clock();
			cout << "Failed on Node " << vertexes.at(id)->getInfo()->getID() << ". See on GraphViewer" << endl;
			timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
			cout << "\nTime taken to check connectivity file: " << timeSpent << endl << endl;
			cout << "\nGraph is not connected\n";			
			system("pause");

			for (Vertex * v : vertexes)
			{
				gv->setVertexColor(v->getInfo()->getID(), v->getInfo()->getColor());
			}
			return false;
		}
	}
	for (Vertex * v : vertexes)
	{
		gv->setVertexColor(v->getInfo()->getID(), GREEN);
	}
	
	end = clock();
	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "\nTime taken to check connectivity file: " << timeSpent << endl << endl;
	cout << "This Graph is connected.See in GraphViewer. ";
	
	system("pause");

	for (Vertex * v : vertexes)
	{
		gv->setVertexColor(v->getInfo()->getID(),v->getInfo()->getColor());
	}
	return true;
}

void SystemManager::isConnectedAux(Vertex * v, int &counter)
{
	counter++;
	v->getInfo()->setVisited(true);
	if(v->getInfo()->isAvailable())
		gv->setVertexColor(v->getInfo()->getID(), BLACK);
	else
		gv->setVertexColor(v->getInfo()->getID(), YELLOW);

	for (Edge e : v->getAdj())
	{
		if (!e.getDest()->getInfo()->getVisited())
			isConnectedAux(e.getDest(),counter);
	}
}

void SystemManager::showClosestLocation(Vertex* origin, int id, bool rent)
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
		waitConfirm();
		paintPath(path, false, 1);
		rent ? ((SharingLocation*)dest)->liftBike() : ((SharingLocation*)dest)->depositBike();
	}
	else
	{
		cout << "Can't reach any sharing location from your location!" << endl;
		waitConfirm();
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
		paintPath(path, false, 1);
		rent ? ((SharingLocation*)dest->getInfo())->liftBike() : ((SharingLocation*)dest->getInfo())->depositBike();
		waitConfirm();
	}
	else
	{
		cout << "Can't reach node " << to_string(dest->getInfo()->getID()) << " from your location!" << endl;
		waitConfirm();
	}
}

bool SystemManager::Menu()
{
	Limpar_ecra();

	selectGraph();

	vector<pair<int, unsigned long long>> idsNodes = loadFiles();

	waitConfirm();

	mainMenu(idsNodes);

	return true;
}

void SystemManager::checkConnectivity()
{
	bool isCon = isConnected();

	if (isCon)
	{
		//TODO biconectivity
	}
}

bool SystemManager::mainMenu(const vector<pair<int, unsigned long long>> &idsNodes)
{
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
		cout << "4 - Check Graph Connectivity" << endl;
		cout << "5 - Exit" << endl;

		int userChoice = verifyInput(1, 5);

		switch (userChoice)
		{
		case 1:
			menuRent();
			break;
		case 2:
			menuHasBike();
			break;
		case 3:
			return true;
		case 4:
			checkConnectivity();
			break;
		case 5:
			menuSave(idsNodes);
			exit(0);
		default:
			break;
		}
	}

	return true;
}

bool SystemManager::menuRent()
{
	Limpar_ecra();

	string location = "";

	cout << "------------------------------" << endl;
	cout << "--------|RENT A BIKE|--------" << endl;
	cout << "------------------------------" << endl;

	while (!isNumber(location))
	{
		cout << endl << "Tell me your location: ";
		getline(cin, location);
	}

	cout << "Enter your preference" << endl;
	cout << "(1) Closest sharing location (no discount)" << endl;
	cout << "(2) Other sharing locations (discount)" << endl;

	int userChoice = verifyInput(1, 2);
	Vertex* loc;
	int id = stoi(location);

	try {
		loc = findLocation(id);
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
		showClosestLocation(loc, id, true);
		break;
	case 2:
		showDiscountLocations(loc, id, true);
		break;
	default:
		break;
	}

	return true;
}

bool SystemManager::menuHasBike()
{
	Limpar_ecra();

	string location = "";

	cout << "------------------------------" << endl;
	cout << "--------|DELIVER A BIKE|--------" << endl;
	cout << "------------------------------" << endl;

	while (!isNumber(location))
	{
		cout << endl << "Tell me your location: ";
		getline(cin, location);
	}

	cout << "Enter your preference" << endl;
	cout << "(1) Closest sharing location (no discount)" << endl;
	cout << "(2) Other sharing locations (discount)" << endl;

	int userChoice = verifyInput(1, 2);

	Vertex* loc;
	int id = stoi(location);

	try {
		loc = findLocation(id);
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
		showClosestLocation(loc, id, false);
		break;
	case 2:
		showDiscountLocations(loc, id, false);
		break;
	default:
		break;
	}

	return true;
}

bool SystemManager::menuSave(const vector<pair<int, unsigned long long>> &idsNodes)
{

	Limpar_ecra();


	cout << "Enter your preference" << endl;
	cout << "(1) Don't save" << endl;
	cout << "(2) Save" << endl;

	int userChoice = verifyInput(1, 2);

	switch (userChoice)
	{
	case 1:
		exit(0);
		break;
	case 2:
		saveSharingLocations(idsNodes);
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
			cout << "Path nao e possivel!!" << endl;
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
	cout << "You can get 50 euros discount if you choose one of the following locations!" << endl;
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

void SystemManager::saveSharingLocations(const vector<pair<int, unsigned long long>> &idsNodes)
{
	vector<Vertex*> sharingLocations;
	vector<Vertex*> vertexSet = graph.getVertexSet();
	ofstream save(fileNames.sharingLocations);

	copy_if(vertexSet.begin(), vertexSet.end(), back_inserter(sharingLocations), [](Vertex* vertex) {
		return strcmp(typeid(*vertex->getInfo()).name(), "class SharingLocation") == 0; });

	int size = sharingLocations.size();

	for (int i = 0; i < size; i++)
	{
		auto id = find_if(idsNodes.begin(), idsNodes.end(), [&sharingLocations, &i](pair<int, unsigned long long> p) {
			return p.first == sharingLocations.at(i)->getInfo()->getID();
		});

		SharingLocation* loc = ((SharingLocation*)sharingLocations.at(i)->getInfo());

		save << to_string(id->second) << ";" << loc->getMaxCapacity() << ";" << loc->getSlots();

		if (i < size - 1)
			save << endl;
	}
	save.close();
	waitConfirm();
}
