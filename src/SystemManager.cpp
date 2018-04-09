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
	cout << "  Welcome !!\n\n";
	cout << " - Choose the Map you want:\n\n";
	cout << TAB << "1 - Baixa Porto          - 763  Nodes, 799  Edges, 20 Sharing Locations\n\n";
	cout << TAB << "2 - Hospital S.Joao/FEUP - 969  Nodes, 1051 Edges, 20 Sharing Locations\n\n";
	cout << TAB << "3 - Porto Paranhos       - 8670 Nodes, 9278 Edges, 50 Sharing Locations\n\n";
	cout << TAB << "4 - Cadinelo Gaia        - 1428 Nodes, 1513 Edges, 30 Sharing Locations\n\n";
	cout << TAB << "5 - Foz Douro Porto      - 2264 Nodes, 2482 Edges, 30 Sharing Locations\n\n";
	cout << TAB << "6 - Trindade, S.Bento    - 4235 Nodes, 4577 Edges, 30 Sharing Locations\n\n";
	cout << " - Test files:\n\n";
	cout << TAB << "7 - Non Connective Graph\n\n";
	cout << TAB << "8 - Connective Graph\n\n";
	cout << TAB << "9 - Biconnective Graph\n\n";

	cout << " - Choose 0 to Quit." << endl << endl;
	int userChoice = verifyInput(0, 9);

	if (!userChoice)
		exit(0);

	graph = Graph();

	if (gv != nullptr)
	{
		gv->closeWindow();
		delete gv;
		cout << "\n - Openning GraphViewer . . .\n\n";
		initGraphViewer();
		gv->createWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	switch (userChoice)
	{
	case 1:
		initFileNames("BaixaPortoNodes.txt",
			"BaixaPortoEdges.txt",
			"BaixaPortoNames.txt",
			"BaixaPortoSharingLocations.txt"
		);
		break;
	case 2:
		initFileNames("FeupNodes.txt",
			"FeupEdges.txt",
			"FeupNames.txt",
			"FeupSharing.txt"
		);
		break;
	case 3:
		initFileNames("PortoNodes.txt",
			"PortoEdges.txt",
			"PortoNomes.txt",
			"PortoSharing.txt"
		);
		break;
	case 4:
		initFileNames("CadineloGaiaNodes.txt",
			"CadineloGaiaEdges.txt",
			"CadineloGaiaNames.txt",
			"CadineloGaiaSharing.txt"
		);
		break;
	case 5:
		initFileNames("FozDouroPortoNodes.txt",
			"FozDouroPortoEdges.txt",
			"FozDouroPortoNames.txt",
			"FozDouroPortoSharing.txt"
		);
		break;
	case 6:
		initFileNames("TrindadeS.BentoPortoNodes.txt",
			"TrindadeS.BentoPortoEdges.txt",
			"TrindadeS.BentoPortoNames.txt",
			"TrindadeS.BentoPortoSharing.txt"
		);
		break;
	case 7:
		initFileNames("nodesNaoConexo.txt",
			"edgesNaoConexo.txt",
			"nomesNaoConexo.txt",
			"sharingLocationsEmpty.txt"
		);
		break;
	case 8:
		initFileNames("nodesConectividade.txt",
			"edgesConectividade.txt",
			"nomesConectividade.txt",
			"sharingLocationsEmpty.txt"
		);
		break;
	case 9:
		initFileNames("nodesBiconectividade.txt",
			"edgesBiconectividade.txt",
			"nomesBiconectividade.txt",
			"sharingLocationsEmpty.txt"
		);
		break;
	default:
		break;
	}
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

unordered_map<int, unsigned long long> SystemManager::loadFiles()
{
	clock_t begin, end;
	double timeSpent;
	vector<SharingLoc> sharingLocations;
	unordered_map<int, unsigned long long> idsNodes;
	vector<EdgeName> edgesNames;

	begin = clock();
	loadSharingLocations(sharingLocations);
	end = clock();
	timeSpent = timeDiff(begin, end);
	cout << " - Time to read Sharing Locations file: " << timeSpent << endl << endl;

	begin = clock();
	loadNodes(idsNodes, sharingLocations);
	end = clock();
	timeSpent = timeDiff(begin, end);
	cout << " - Time to read Nodes file: " << timeSpent << endl << endl;

	begin = clock();
	edgesNames = loadNames();
	end = clock();
	timeSpent = timeDiff(begin, end);
	cout << " - Time to read Names file: " << timeSpent << endl << endl;

	begin = clock();
	loadEdges(edgesNames, idsNodes);
	end = clock();
	timeSpent = timeDiff(begin, end);
	cout << " - Time to read Edges file: " << timeSpent << endl << endl;

	gv->rearrange();

	return idsNodes;
}

void SystemManager::loadSharingLocations(vector<SharingLoc> &sharingLocations)
{
	ifstream file(fileNames.sharingLocations);

	cout << " - File: " << fileNames.sharingLocations << endl;

	if (!file.is_open())
	{
		cerr << endl << " - File not found!" << endl;
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

	cout << " - File: " << fileNames.nodes << endl;

	if (!read.is_open())
	{
		cerr << endl << " - File not found!" << endl;
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
	cout << " - File: " << fileNames.names << endl;

	vector<EdgeName> edges;

	if (!read.is_open())
	{
		cerr << endl << " - File not found!" << endl;
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

	cout << " - File: " << fileNames.edges << endl;

	unsigned long long id = -1, ori, dest;
	char ign;

	if (!read.is_open())
	{
		cerr << endl << " - File not found!" << endl;
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

bool SystemManager::mainMenu(const unordered_map<int, unsigned long long> &idsNodes)
{
	while (true)
	{
		Limpar_ecra();

		// main menu
		cout << " _______________________________ " << endl;
		cout << "|                               |" << endl;
		cout << "|           M  E  N  U          |" << endl;
		cout << "|_______________________________|" << endl;
		cout << "|                               |" << endl;
		cout << "| 1 - Rent a bike               |" << endl;
		cout << "|                               |" << endl;
		cout << "| 2 - I already have one        |" << endl;
		cout << "|                               |" << endl;
		cout << "| 3 - Check Graph Connectivity  |" << endl;
		cout << "|                               |" << endl;
		cout << "| 4 - Select new graph          |" << endl;
		cout << "|                               |" << endl;
		cout << "| 5 - Exit                      |" << endl;
		cout << "|_______________________________|" << endl << endl;

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
			checkConnectivity();
			break;
		case 4:
			return true;
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

	cout << " _______________________________ " << endl;
	cout << "|                               |" << endl;
	cout << "|    R E N T    A    B I K E    |" << endl;
	cout << "|_______________________________|" << endl;

	int wrongInput = 0;

	while (!isNumber(location))
	{
		if (wrongInput)
			cout << " - Wrong Input . . ." << endl;

		cout << endl << " - Tell me your location: ";

		getline(cin, location);
		wrongInput++;
	}

	int id = stoi(location);
	Vertex* loc;

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
		cout << "\n - Unknown exception." << endl;

		waitConfirm();

		return false;
	}

	gv->setVertexColor(id, YELLOW);

	cout << endl << endl << " - You can see you location on Graph Viewer, Green Node" << endl << endl;
	cout << " - Enter your preference:" << endl << endl;
	cout << TAB << TAB << "1 -> Closest sharing location (no discount)" << endl << endl;
	cout << TAB << TAB << "2 -> Other sharing locations (discount)" << endl << endl;

	int userChoice = verifyInput(1, 2);

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

	gv->setVertexColor(id, loc->getInfo()->getColor());

	return true;
}



bool SystemManager::menuHasBike()
{
	Limpar_ecra();

	string location = "";

	cout << " _______________________________ " << endl;
	cout << "|                               |" << endl;
	cout << "|   D E L I V E R  A  B I K E   |" << endl;
	cout << "|_______________________________|" << endl;

	int wrongInput = 0;

	while (!isNumber(location))
	{
		if (wrongInput)
			cout << " - Wrong Input . . ." << endl;

		cout << endl << " - Tell me your location: ";

		getline(cin, location);
		wrongInput++;
	}

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
		cout << "\n - Unknown exception." << endl;
		return false;
	}

	gv->setVertexColor(id, YELLOW);

	cout << endl << endl << " - You can see you location on Graph Viewer, Green Node" << endl << endl;
	cout << " - Enter your preference:" << endl << endl;
	cout << TAB << TAB << "1 -> Closest sharing location (no discount)" << endl << endl;
	cout << TAB << TAB << "2 -> Other sharing locations (discount)" << endl << endl;

	int userChoice = verifyInput(1, 2);


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

	gv->setVertexColor(id, loc->getInfo()->getColor());

	return true;
}

void SystemManager::showClosestLocation(Vertex *origin, int id, bool rent)
{
	clock_t begin, end;

	Location *dest = NULL;

	begin = clock();
	bool success = graph.dijkstraShortestPath(origin->getInfo(), dest, rent);
	end = clock();

	if (success)
	{
		vector<Vertex> path = graph.getPath(origin->getInfo(), dest);
		gv->setVertexColor(id, YELLOW);
		paintPath(path, true, 5, START_NODE_COLOR, RED, GREEN, PATH_COLOR);

		double timeSpent = timeDiff(begin, end);
		cout << endl << " - Found closest location in: " << to_string(timeSpent) << " seconds!" << endl;

		// VER ISTO
		//cout << "Travel will take you approximately " << getTime(getPathLength(path)) << endl;

		cout << endl << " - Check the map to see the generated path!" << endl;
		waitConfirm();
		paintPath(path, false, 1);
		rent ? ((SharingLocation *)dest)->liftBike() : ((SharingLocation *)dest)->depositBike();
	}
	else
	{
		cout << endl << " - Can't reach any sharing location from your location!" << endl;
		waitConfirm();
	}
}

bool SystemManager::menuSave(const unordered_map<int, unsigned long long> &idsNodes)
{

	Limpar_ecra();

	cout << " _______________________________ " << endl;
	cout << "|                               |" << endl;
	cout << "|    S A V E  C H A N G E S     |" << endl;
	cout << "|_______________________________|" << endl;
	cout << "|                               |" << endl;
	cout << "| 1 - Don't save                |" << endl;
	cout << "|                               |" << endl;
	cout << "| 2 - Save                      |" << endl;
	cout << "|_______________________________|" << endl << endl;

	int userChoice = verifyInput(1, 2);

	cout << endl;

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

	cout << endl << " - Sharing Locations info saved" << endl << endl;
	cout << "Exiting . . ." << endl;

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

void SystemManager::paintPath(vector<Vertex> path, bool def, int edgeThickness, string startNodeColor, string endNodeColor, string elseNodeColor, string edgeColor)
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
		for (vector<Vertex>::iterator v = path.begin(); v != path.end(); v++)
		{
			if (v == path.begin())
				gv->setVertexColor(v->getInfo()->getID(), startNodeColor);
			else if (v == path.end() - 1)
				gv->setVertexColor(v->getInfo()->getID(), endNodeColor);
			else
				gv->setVertexColor(v->getInfo()->getID(), elseNodeColor);
		}

	}
	else
	{
		for (auto v : path)
		{
			gv->setVertexColor(v.getInfo()->getID(), v.getInfo()->getColor());
		}
	}
}

void SystemManager::showDiscountLocations(Vertex *origin, int id, bool rent)
{
	vector<Vertex *> v = graph.discountLocations(rent, DISCOUNT_LOCATIONS);

	vector<Vertex> path = getDiscountChoice(v, origin);

	paintPath(path, true, 5, START_NODE_COLOR, END_NODE_COLOR, GREEN, PATH_COLOR);

	cout << " - Check the map to see your travel route!" << endl;

	waitConfirm();

	paintPath(path, false, 1);

	rent ? ((SharingLocation *)path.back().getInfo())->liftBike() : ((SharingLocation *)path.back().getInfo())->depositBike();
}

vector<Vertex> SystemManager::getDiscountChoice(const vector<Vertex *> &v, Vertex *origin) const
{
	cout << endl << " - You can get a discount if you choose one of the following locations!" << endl;
	cout << endl << " - Here are their IDs: " << endl;

	int userChoice;
	string input;
	auto exists = v.end();
	map<int, vector<Vertex*>> paths;
	int size = v.size();

	//Fazer dijkstra e verificar se há path
	for (auto it : v)
	{
		//Location *origin, Vertex *destiny
		/*if(graph.dijkstraShortestPath(*origin->getInfo(), *it))
		{
			gv->setVertexColor(it->getInfo()->getID(), "PINK");
			//paths.insert(it->getInfo()->getID(), graph.getPath(origin, it->getInfo()));
		}*/
	}

	//Mostrar sharingLocations com path
	for (int i = 0; i < size; i++)
	{
		if (paths.find(v[i]->getInfo()->getID()) != paths.end())
		{
			cout << " - Sharing Location: " << v[i]->getInfo()->getID()
				<< " Discount: "
				;// << getIncentive(getPathLength(paths.at(i))) << "%%" << endl;
		}
	}

	//Esperar por ID que exista
	while (exists == v.end())
	{
		cout << " - Enter your preference: ";
		getline(cin, input);

		if (isNumber(input))
		{
			userChoice = stoi(input);
			/*exists = find_if(paths.begin(), paths.end(), [userChoice](auto el) {
				return el->first == userChoice;
			});*/
		}
	}

	//return paths.find(userChoice);
	vector<Vertex> a;
	return a;
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
}

double SystemManager::getPathLength(const vector<Vertex*> &path)
{
	double length = 0;

	for (auto it : path)
		length += it->getDist();

	return length;
}

bool SystemManager::isConnected()
{
	Limpar_ecra();
	double timeSpent;
	clock_t begin, end;
	begin = clock();

	cout << " - Checking graph connectivity...\n";

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
			cout << "\n -	Failed on Node " << vertexes.at(id)->getInfo()->getID() << endl;
			cout << "\n - See on GraphViewer" << endl;
			timeSpent = timeDiff(begin, end);
			cout << "\n - Time taken to check connectivity file: " << timeSpent << endl;
			cout << "\n - Graph is not connected\n";
			
			waitConfirm();

			for (unsigned int i = 0; i <= id; i++)
				gv->setVertexColor(i + 1, vertexes.at(i)->getInfo()->getColor());
			return false;
		}
	}

	for (Vertex *v : vertexes)
	{
		gv->setVertexColor(v->getInfo()->getID(), v->getInfo()->getColor());
	}

	end = clock();
	timeSpent = timeDiff(begin, end);
	cout << "\nTime taken to check connectivity file: " << timeSpent << endl
		<< endl;
	cout << "This Graph is connected.See in GraphViewer. ";

	string quit = "";


	for (unsigned int id = 0; id < vertexes.size(); id++)
	{
		if (id == vertexes.size())
			id = 0;

		for (Vertex *v : vertexes)
		{
			v->getInfo()->setVisited(false);
		}

		connectedCiclePaint(vertexes.at(id));

		Sleep(2000);
		for (Vertex *v : vertexes)
		{
			gv->setVertexColor(v->getInfo()->getID(), v->getInfo()->getColor());
		}
		Sleep(2000);
	}
	waitConfirm();

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

	for (Edge e : v->getAdj())
	{
		if (!e.getDest()->getInfo()->getVisited())
			isConnectedAux(e.getDest(), counter);
	}
}

void SystemManager::connectedCiclePaint(Vertex *v)
{
	v->getInfo()->setVisited(true);
	Sleep(1000);
	gv->setVertexColor(v->getInfo()->getID(), RED);

	for (Edge e : v->getAdj())
	{
		if (!e.getDest()->getInfo()->getVisited())
			connectedCiclePaint(e.getDest());
	}
}

void SystemManager::checkConnectivity()
{
	bool isCon = isConnected();

	if (isCon)
	{
		//TODO biconectivity
	}
}