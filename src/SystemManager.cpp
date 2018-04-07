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

SystemManager::SystemManager()
{
	initGraphViewer();
}

void SystemManager::initGraphViewer()
{
	gv = new GraphViewer(WINDOW_WIDTH, WINDOW_HEIGHT, false);
	gv->defineEdgeColor(EDGE_COLOR_DEFAULT);
	gv->defineVertexColor(VERTEX_COLOR_DEFAULT);
}

SystemManager::~SystemManager()
{
	delete gv;
}

void SystemManager::initFileNames(string nodes, string edges, string names, string sharing)
{
	this->fileNames.nodes = nodes;
	this->fileNames.edges = edges;
	this->fileNames.names = names;
	this->fileNames.sharingLocations = sharing;
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
		cout << "\nOpenning GraphViewer . . .\n\n";
		initGraphViewer();
		gv->createWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	switch (userChoice)
	{
	case 1:
		initFileNames("AHnodes.txt", "AHedges.txt", "AHnames.txt", "AHsharingLocations.txt");
		break;
	case 2:
		initFileNames("FeupNodes.txt", "FeupEdges.txt", "FeupNames.txt", "FeupSharing.txt");
		break;
	case 3:
		initFileNames("PortoNodes.txt", "PortoEdges.txt", "PortoNomes.txt", "PortoSharing.txt");
		break;
	case 4:
		initFileNames("Conetividade//nodesNaoConexo.txt", "Conetividade//edgesNaoConexo.txt",
					  "Conetividade//nomesNaoConexo.txt", "Conetividade//sharingLocationsEmpty.txt");
		break;
	case 5:
		initFileNames("nodesConectividade.txt", "edgesConectividade.txt",
					  "nomesConectividade.txt", "sharingLocationsEmpty.txt");
		break;
	case 6:
		initFileNames("//Conetividade//nodesBiconectividade.txt", "//Conetividade//nodesBiconectividade.txt",
					  "//Conetividade//nomesBiconectividade.txt", "//Conetividade//sharingLocationsEmpty.txt");
		break;
	default:
		break;
	}
}

unordered_map<int, unsigned long long> SystemManager::loadFiles()
{
	clock_t begin, end;
	double timeSpent;
	vector<SharingLoc> sharingLocations;

	begin = clock();
	loadSharingLocations(sharingLocations);
	end = clock();

	timeSpent = timeDiff(begin, end);
	cout << "Time to read Sharing Locations file: " << timeSpent << endl
		 << endl;

	unordered_map<int, unsigned long long> idsNodes;

	begin = clock();
	loadNodes(idsNodes, sharingLocations);
	end = clock();

	timeSpent = timeDiff(begin, end);
	cout << "Time to read Nodes file: " << timeSpent << endl
		 << endl;

	begin = clock();
	vector<EdgeName> edgesNames = loadNames();
	end = clock();

	timeSpent = timeDiff(begin, end);
	cout << "Time to read Names file: " << timeSpent << endl
		 << endl;

	begin = clock();
	loadEdges(edgesNames, idsNodes);
	end = clock();

	timeSpent = timeDiff(begin, end);
	cout << "Time to read Edges file: " << timeSpent << endl
		 << endl;

	gv->rearrange();

	return idsNodes;
}

void SystemManager::loadSharingLocations(vector<SharingLoc> &sharingLocations)
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

void SystemManager::loadNodes(unordered_map<int, unsigned long long> &idsNodes, const vector<SharingLoc> &sharingLocations)
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

			idsNodes.insert(make_pair(idInt, id));

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

void SystemManager::loadEdges(vector<EdgeName> &edges, unordered_map<int, unsigned long long> &idsNode)
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

			Vertex *origin;
			Vertex *destiny;
			int origemID = 0, destinoID = 0;

			auto itOrigin = find_if(idsNode.begin(), idsNode.end(), [ori](auto inf) {
				return inf.second == ori;
			});

			origin = graph.findVertex(new Location(itOrigin->first));
			origemID = itOrigin->first;

			auto itDest = find_if(idsNode.begin(), idsNode.end(), [dest](auto inf) {
				return inf.second == dest;
			});

			destiny = graph.findVertex(new Location(itDest->first));
			destinoID = itDest->first;

			if (destiny == nullptr || origin == nullptr)
			{
				cerr << "\nError on Edges file. Some Location(node) is not defined. File could have been corrupted." << endl
					 << endl;
				exit(4);
			}

			double weight = calcWeight((origin->getInfo()), (destiny->getInfo()));

			auto x = find_if(edges.begin(), edges.end(), [id](EdgeName &edge) {
				return edge.id == id;
			});

			if (x != edges.end())
			{
				if (x->isBidirectional)
				{
					gv->addEdge(idIntEdge, origemID, destinoID, EdgeType::UNDIRECTED);
					graph.addEdge(origin->getInfo(), destiny->getInfo(), weight, idIntEdge, x->name);
					gv->setEdgeLabel(idIntEdge, x->name);
					idIntEdge++;
					graph.addEdge(destiny->getInfo(), origin->getInfo(), weight, idIntEdge, x->name);
				}
				else
				{
					gv->addEdge(idIntEdge, origemID, destinoID, EdgeType::DIRECTED);
					graph.addEdge(origin->getInfo(), destiny->getInfo(), weight, idIntEdge, x->name);
					gv->setEdgeLabel(idIntEdge, x->name);
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

		for (Vertex *v : vertexes)
		{
			v->getInfo()->setVisited(false);
		}

		isConnectedAux(vertexes.at(id), counter);
		gv->setVertexColor(id + 1, MAGENTA);

		if (counter != vertexes.size())
		{
			end = clock();
			cout << "Failed on Node " << vertexes.at(id)->getInfo()->getID() << ". See on GraphViewer" << endl;
			timeSpent = timeDiff(begin, end);
			cout << "\nTime taken to check connectivity file: " << timeSpent << endl
				 << endl;
			cout << "\nGraph is not connected\n";
			system("pause");

			for (unsigned int i = 0; i <= id; i++)
				gv->setVertexColor(i + 1, vertexes.at(i)->getInfo()->getColor());
			return false;
		}
	}
	for (Vertex *v : vertexes)
	{
		gv->setVertexColor(v->getInfo()->getID(), YELLOW);
	}

	end = clock();
	timeSpent = timeDiff(begin, end);
	cout << "\nTime taken to check connectivity file: " << timeSpent << endl
		 << endl;
	cout << "This Graph is connected.See in GraphViewer. ";

	system("pause");

	for (Vertex *v : vertexes)
	{
		gv->setVertexColor(v->getInfo()->getID(), v->getInfo()->getColor());
	}
	return true;
}

void SystemManager::isConnectedAux(Vertex *v, int &counter)
{
	counter++;
	v->getInfo()->setVisited(true);
	//gv->setVertexColor(v->getInfo()->getID(), YELLOW);

	for (Edge e : v->getAdj())
	{
		if (!e.getDest()->getInfo()->getVisited())
			isConnectedAux(e.getDest(), counter);
	}
}

void SystemManager::showClosestLocation(Vertex *origin, int id, bool rent)
{
	clock_t begin, end;
	begin = clock();

	Vertex *dest = NULL;
	bool success = graph.dijkstraShortestPath(origin->getInfo(), dest);

	end = clock();
	double timeSpent = timeDiff(begin, end);

	if (success)
	{
		vector<Vertex> path = graph.getPath(origin->getInfo(), dest->getInfo());
		gv->setVertexColor(id, YELLOW);
		paintPath(path, true, 5, START_NODE_COLOR, END_NODE_COLOR, PATH_COLOR);
		cout << "Found closest location in: " << to_string(timeSpent) << " seconds!" << endl;
		cout << "Travel will take you approximately " << getTime(getPathLength(path)) << endl;
		cout << "Check the map to see the generated path!" << endl;
		waitConfirm();
		paintPath(path, false, 1);
		rent ? ((SharingLocation *)dest)->liftBike() : ((SharingLocation *)dest)->depositBike();
	}
	else
	{
		cout << "Can't reach any sharing location from your location!" << endl;
		waitConfirm();
	}
}

void SystemManager::showDiscountLocations(Vertex *origin, int id, bool rent)
{
	vector<Vertex *> v = graph.discountLocations(rent, DISCOUNT_LOCATIONS);
	vector<Vertex> path = getDiscountChoice(v);

	paintPath(path, true, 5, START_NODE_COLOR, END_NODE_COLOR, PATH_COLOR);
	cout << "Check the map to see your travel route!" << endl;
	waitConfirm();
	paintPath(path, false, 1);
	rent ? ((SharingLocation *)dest->getInfo())->liftBike() : ((SharingLocation *)dest->getInfo())->depositBike();
}

bool SystemManager::Menu()
{
	Limpar_ecra();

	selectGraph();

	unordered_map<int, unsigned long long> idsNodes = loadFiles();

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

bool SystemManager::mainMenu(const unordered_map<int, unsigned long long> &idsNodes)
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
		cout << endl
			 << "Tell me your location: ";
		getline(cin, location);
	}

	cout << "Enter your preference" << endl;
	cout << "(1) Closest sharing location (no discount)" << endl;
	cout << "(2) Other sharing locations (discount)" << endl;

	int userChoice = verifyInput(1, 2);
	Vertex *loc;
	int id = stoi(location);

	try
	{
		loc = findLocation(id);
	}
	catch (LocationNotFound &e)
	{
		cerr << e.message();

		waitConfirm();

		return false;
	}
	catch (...)
	{
		cout << "Unknown exception." << endl;

		waitConfirm();

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
		cout << endl
			 << "Tell me your location: ";
		getline(cin, location);
	}

	cout << "Enter your preference" << endl;
	cout << "(1) Closest sharing location (no discount)" << endl;
	cout << "(2) Other sharing locations (discount)" << endl;

	int userChoice = verifyInput(1, 2);

	Vertex *loc;
	int id = stoi(location);

	try
	{
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

bool SystemManager::menuSave(const unordered_map<int, unsigned long long> &idsNodes)
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
Vertex *SystemManager::findLocation(const int ID) const
{
	vector<Vertex *> set = graph.getVertexSet();

	auto it = find_if(set.begin(), set.end(), [ID](Vertex *v) {
		return v->getInfo()->getID() == ID;
	});

	if (it != set.end())
		return *it;

	throw LocationNotFound(ID);
}

int SystemManager::convertLongitudeToX(float longitude)
{
	return floor(((longitude - MIN_LON) * (WINDOW_HEIGHT)) / (MAX_LON - MIN_LON));
}

int SystemManager::convertLatitudeToY(float latitude)
{
	return floor(((latitude - MIN_LAT) * (WINDOW_WIDTH)) / (MAX_LAT - MIN_LAT));
}

void SystemManager::paintPath(vector<Vertex> path, bool def, int edgeThickness, string startNodeColor, string endNodeColor, string edgeColor)
{
	if (path.size() < 1)
	{
		cout << "Path not found.\nAre you sure there is a connection?\n";
		return;
	}
	for (int i = 0; i < path.size() - 1; i++)
	{
		Edge edge = graph.findEdge(path[i].getInfo(), path[i + 1].getInfo());

		if (edge.getID() == -1)
		{
			cout << "Path isn't possible!!" << endl;
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

vector<Vertex> SystemManager::getDiscountChoice(const vector<Vertex *> &v) const
{
	cout << "You can get a discount if you choose one of the following locations!" << endl;
	cout << "Here are their IDs: " << endl;

	int userChoice;
	string input;
	auto exists = v.end();
	map<int, vector<Vertex>> paths;
	int size = v.size();

	//Fazer dijkstra e verificar se há path
	for (auto it : v)
	{
		bool success = graph.dijkstraShortestPath(origin, *it);
		if (success)
		{
			gv->setVertexColor(it->getInfo()->getID(), "PINK");
			paths.insert(it->getInfo()->getID(), graph.getPath(origin, it->getInfo()));
		}
	}

	//Mostrar sharingLocations com path
	for (int i = 0; i < size; i++)
	{
		if (paths.find(v[i]->getInfo()->getID() != paths.end()))
		{
			cout << "Sharing Location: " << v[i]->getInfo()->getID()
				 << "Discount: "
				 << getIncentive(getPathLength(paths.at(i))) << "%%" << endl;
		}
	}

	//Esperar por ID que exista
	while (exists == v.end())
	{
		cout << "Enter your preference: ";
		getline(cin, input);

		if (isNumber(input))
		{
			userChoice = stoi(input);
			exists = find_if(paths.begin(), paths.end(), [userChoice](auto el) {
				return el->first == userChoice;
			});
		}
	}

	return *paths.find(userChoice);
}

void SystemManager::saveSharingLocations(const unordered_map<int, unsigned long long> &idsNodes)
{
	vector<Vertex *> sharingLocations;
	vector<Vertex *> vertexSet = graph.getVertexSet();
	ofstream save(fileNames.sharingLocations);

	copy_if(vertexSet.begin(), vertexSet.end(), back_inserter(sharingLocations), [](Vertex *vertex) { return strcmp(typeid(*vertex->getInfo()).name(), "class SharingLocation") == 0; });

	int size = sharingLocations.size();

	for (int i = 0; i < size; i++)
	{
		auto id = find_if(idsNodes.begin(), idsNodes.end(), [&sharingLocations, &i](auto p) {
			return p.first == sharingLocations.at(i)->getInfo()->getID();
		});

		SharingLocation *loc = ((SharingLocation *)sharingLocations.at(i)->getInfo());

		save << to_string(id->second) << ";" << loc->getMaxCapacity() << ";" << loc->getSlots();

		if (i < size - 1)
			save << endl;
	}
	save.close();
	waitConfirm();
}

double SystemManager::getPathLength(const vector<Vertex> &path)
{
	double length = 0;

	for (auto it : path)
		length += it.getDist();

	return length;
}
