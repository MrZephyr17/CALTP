#include "SystemManager.h"
#include "Location.h"
#include "SharingLocation.h"
#include "Exceptions.h"
#include "Utils.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <map>

using namespace std;

SystemManager::~SystemManager()
{
	delete gv;
}

int SystemManager::selectGraph()
{
	cout << " __________________________________________________________________________________ " << endl;
	cout << "|                                                                                  |" << endl;
	cout << "|                                 W E L C O M E !                                  |" << endl;
	cout << "|__________________________________________________________________________________|" << endl;
	cout << "|                                                                                  |" << endl;
	cout << "|  Choose the Map you want :                                                       |" << endl;
	cout << "|                                                                                  |" << endl;
	cout << "|      1 - Baixa Porto          -  763 Nodes,  799 Edges, 20 Sharing Locations     |" << endl;
	cout << "|                                                                                  |" << endl;
	cout << "|      2 - Hospital S.Joao/FEUP -  969 Nodes, 1051 Edges, 20 Sharing Locations     |" << endl;
	cout << "|                                                                                  |" << endl;
	cout << "|      3 - Porto Paranhos       - 8670 Nodes, 9278 Edges, 50 Sharing Locations     |" << endl;
	cout << "|                                                                                  |" << endl;
	cout << "|      4 - Canidelo Gaia        - 1428 Nodes, 1513 Edges, 30 Sharing Locations     |" << endl;
	cout << "|                                                                                  |" << endl;
	cout << "|      5 - Foz Douro Porto      - 2264 Nodes, 2482 Edges, 30 Sharing Locations     |" << endl;
	cout << "|                                                                                  |" << endl;
	cout << "|      6 - Trindade, S.Bento    - 4235 Nodes, 4577 Edges, 30 Sharing Locations     |" << endl;
	cout << "|__________________________________________________________________________________|" << endl;
	cout << "|                                                                                  |" << endl;
	cout << "|  Test files:                                                                     |" << endl;
	cout << "|                                                                                  |" << endl;
	cout << "|      7 - Non Connective Graph -    8 Nodes,   11 Edges   0 Sharing Locations     |" << endl;
	cout << "|                                                                                  |" << endl;
	cout << "|      8 - Connective Graph     -   10 Nodes,   16 Edges,  0 Sharing Locations     |" << endl;
	cout << "|__________________________________________________________________________________|" << endl
		 << endl;

	cout << " - Choose 0 to Quit." << endl
		 << endl;
	int userChoice = verifyInput(0, 8);

	if (!userChoice)
	{
		cout << endl;
		exit(0);
	}

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
					  "BaixaPortoSharingLocations.txt");
		break;
	case 2:
		initFileNames("FeupNodes.txt",
					  "FeupEdges.txt",
					  "FeupNames.txt",
					  "FeupSharing.txt");
		break;
	case 3:
		initFileNames("PortoNodes.txt",
					  "PortoEdges.txt",
					  "PortoNomes.txt",
					  "PortoSharing.txt");
		break;
	case 4:
		initFileNames("CanideloGaiaNodes.txt",
					  "CanideloGaiaEdges.txt",
					  "CanideloGaiaNames.txt",
					  "CanideloGaiaSharing.txt");
		break;
	case 5:
		initFileNames("FozDouroPortoNodes.txt",
					  "FozDouroPortoEdges.txt",
					  "FozDouroPortoNames.txt",
					  "FozDouroPortoSharing.txt");
		break;
	case 6:
		initFileNames("TrindadeS.BentoPortoNodes.txt",
					  "TrindadeS.BentoPortoEdges.txt",
					  "TrindadeS.BentoPortoNames.txt",
					  "TrindadeS.BentoPortoSharing.txt");
		break;
	case 7:
		initFileNames("nodesNaoConexo.txt",
					  "edgesNaoConexo.txt",
					  "nomesNaoConexo.txt",
					  "sharingLocationsEmpty.txt");
		break;
	case 8:
		initFileNames("nodesConectividade.txt",
					  "edgesConectividade.txt",
					  "nomesConectividade.txt",
					  "sharingLocationsEmpty.txt");
		break;
	default:
		break;
	}

	return userChoice;
}

bool SystemManager::Menu()
{
	clearScreen();

	int option = selectGraph();

	unordered_map<int, unsigned long long> idsNodes = loadFiles(option);

	waitConfirm();

	mainMenu(idsNodes);

	return true;
}

bool SystemManager::mainMenu(const unordered_map<int, unsigned long long> &idsNodes)
{
	while (true)
	{
		clearScreen();

		cout << " _______________________________ " << endl;
		cout << "|                               |" << endl;
		cout << "|           M  E  N  U          |" << endl;
		cout << "|_______________________________|" << endl;
		cout << "|                               |" << endl;
		cout << "| 1 - Rent a bike               |" << endl;
		cout << "|                               |" << endl;
		cout << "| 2 - I already have one        |" << endl;
		cout << "|                               |" << endl;
		cout << "| 3 - Find sharing location     |" << endl;
		cout << "|                               |" << endl;
		cout << "| 4 - Check Graph Connectivity  |" << endl;
		cout << "|                               |" << endl;
		cout << "| 5 - Visualize SH information  |" << endl;
		cout << "|                               |" << endl;
		cout << "| 6 - Select new graph          |" << endl;
		cout << "|                               |" << endl;
		cout << "| 7 - Exit                      |" << endl;
		cout << "|_______________________________|" << endl
			 << endl;

		int userChoice = verifyInput(1, 7);

		switch (userChoice)
		{
		case 1:
			menuRent();
			break;
		case 2:
			menuHasBike();
			break;
		case 3:
			menuFindSharingLocation();
			break;
		case 4:
			checkConnectivity();
			break;
		case 5:
			sharingLocationsInfo();
			break;
		case 6:
			return true;
		case 7:
			menuSave(idsNodes);
			exit(0);
		default:
			break;
		}
	}

	return true;
}

void SystemManager::menuFindSharingLocation()
{
	clearScreen();

	cout << " ______________________________________________ " << endl;
	cout << "|                               				|" << endl;
	cout << "|    F I N D  S H A R I N G  L O C A T I O N   |" << endl;
	cout << "|______________________________________________|" << endl << endl;

	string street1, street2;
	getNames(street1, street2);

	cout << endl << "Enter your preference: " << endl;
	cout << "1 - Exact search" << endl;
	cout << "2 - Approximate search" << endl << endl;

	int userChoice = verifyInput(1, 2);

	switch (userChoice)
	{
	case 1:
		findSLExact(street1, street2);
		break;
	case 2:
		findSLApproximate(street1, street2);
		break;
	default:
		break;
	}
}

void SystemManager::findSLExact(string street1, string street2)
{
	Vertex *location = nullptr;
	clock_t begin, end;

	begin = clock();
	bool foundStreets = graph.findSLExact(street1, street2, location);
	end = clock();

	double timeSpent = timeDiff(begin, end);

	if (!foundStreets)
	{
		cout << endl << "Unknown location!" << endl;
		cout << endl << "Took a total of: " << timeSpent << " seconds." << endl;
		waitConfirm();
		return;
	}
	else if (location == nullptr)
	{
		cout << endl << "Couldn't find a location in that crossroad!" << endl;
		cout << endl << "Took a total of: " << timeSpent << " seconds." << endl;
		waitConfirm();
		return;
	}

	gv->setVertexColor(location->getInfo()->getID(), RED);
	cout << "Location found!" << endl;
	cout << "It is now on the map with the red color!" << endl;
	cout << endl << "Took a total of: " << timeSpent << " seconds." << endl;

	waitConfirm();
}

void SystemManager::findSLApproximate(string street1, string street2)
{	
	clock_t begin, end;

	begin = clock();
	multimap<int,string> streets = graph.findSLApproximate(street1, street2);
	end = clock();

	double timeSpent = timeDiff(begin, end);

	if (streets.size() == 0)
	{
		cout << "Sorry. Couldn't find any similar street that have Sharing Locations." << endl;
		cout << endl << "Took a total of: " << timeSpent << " seconds." << endl;
		waitConfirm();
		return;
	}

	cout << endl << "Similar streets that have Sharing Locations" << endl << endl;

	for (auto it = streets.begin(); it != streets.end(); it++)
	{
		cout << it->second << endl;
	}

	cout << endl << "Took a total of: " << timeSpent << " seconds." << endl;

	waitConfirm();
}

bool SystemManager::menuRent()
{
	clearScreen();

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

		cout << endl
			 << " - Tell me your location: ";

		getline(cin, location);
		wrongInput++;
	}

	int id = stoi(location);
	Vertex *loc;

	try
	{
		loc = graph.findLocation(id);
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

	if (loc->getInfo()->getColor() == "RED")
		cout << endl
			 << " - Your current position is already a Sharing Location" << endl;
	else
		gv->setVertexColor(id, YELLOW);

	gv->rearrange();

	cout << endl
		 << " - You can see you location on Graph Viewer (Red if Sharing Location, Yellow otherwise)" << endl
		 << endl;
	cout << " - Enter your preference:" << endl
		 << endl;
	cout << TAB << TAB << "1 -> Closest sharing location (no discount)" << endl
		 << endl;
	cout << TAB << TAB << "2 -> Other sharing locations (discount)" << endl
		 << endl;

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
	gv->rearrange();

	return true;
}

bool SystemManager::menuHasBike()
{
	clearScreen();

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

		cout << endl
			 << " - Tell me your location: ";

		getline(cin, location);
		wrongInput++;
	}

	Vertex *loc;
	int id = stoi(location);

	try
	{
		loc = graph.findLocation(id);
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

	if (loc->getInfo()->getColor() == "RED")
	{
		cout << endl
			 << endl
			 << " - Your current position is already a Sharing Location" << endl;

		waitConfirm();

		return true;
	}

	if (loc->getInfo()->getColor() == "RED")
		cout << endl
			 << " - Your current position is already a Sharing Location" << endl;
	else
		gv->setVertexColor(id, YELLOW);

	gv->rearrange();

	cout << endl
		 << endl
		 << " - You can see you location on Graph Viewer, Yellow Node" << endl
		 << endl;
	cout << " - Enter your preference:" << endl
		 << endl;
	cout << TAB << TAB << "1 -> Closest sharing location (no discount)" << endl
		 << endl;
	cout << TAB << TAB << "2 -> Other sharing locations (discount)" << endl
		 << endl;

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
	gv->rearrange();

	return true;
}

void SystemManager::showClosestLocation(Vertex *origin, int id, bool rent)
{
	clock_t begin, end;
	Vertex *dest = NULL;

	begin = clock();
	bool success = graph.dijkstraShortestPath(origin->getInfo(), dest, rent);
	end = clock();

	if (success)
	{
		vector<Vertex> path = graph.getPath(origin->getInfo(), dest->getInfo());

		gv->setVertexColor(id, YELLOW);

		paintPath(path, true, 5, START_NODE_COLOR, RED, GREEN, PATH_COLOR);

		double timeSpent = timeDiff(begin, end);

		cout << endl
			 << " - Found closest location in: " << to_string(timeSpent) << " seconds!" << endl;

		cout << endl
			 << " - Travel will take you approximately " << getTime(dest->getDist()) << endl;

		cout << endl
			 << " - Check the map to see the generated path!" << endl;

		cout << endl
			 << TAB << "  - Yellow Node is the given position" << endl;

		cout << endl
			 << TAB << "  - Green Nodes are the path to destiny" << endl;

		cout << endl
			 << TAB << "  - Red Node right next to Green Node is your destiny" << endl;

		cout << endl
			 << TAB;

		SharingLocation *shLoc = (SharingLocation *)dest->getInfo();

		rent ? cout << "  - You have lifted a bike!" << endl : cout << "  - You have deposited a bike!" << endl;

		waitConfirm();

		paintPath(path, false, 1);
	}
	else
	{
		cout << endl
			 << " - Can't reach any sharing location from your location!" << endl;

		waitConfirm();
	}
}

bool SystemManager::menuSave(const unordered_map<int, unsigned long long> &idsNodes)
{
	clearScreen();

	cout << " _______________________________ " << endl;
	cout << "|                               |" << endl;
	cout << "|    S A V E  C H A N G E S     |" << endl;
	cout << "|_______________________________|" << endl;
	cout << "|                               |" << endl;
	cout << "| 1 - Don't save                |" << endl;
	cout << "|                               |" << endl;
	cout << "| 2 - Save                      |" << endl;
	cout << "|_______________________________|" << endl
		 << endl;

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

	cout << endl
		 << " - Sharing Locations info saved" << endl
		 << endl;
	cout << "Exiting . . ." << endl;

	return true;
}

void SystemManager::paintPath(vector<Vertex> path, bool def, int edgeThickness, string startNodeColor, string endNodeColor, string elseNodeColor, string edgeColor)
{
	unsigned int size = path.size();

	if (size < 1)
	{
		cout << "Path not found.\nAre you sure there is a connection?\n";
		return;
	}
	for (unsigned int i = 0; i < size - 1; i++)
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
			if (v->getInfo()->getColor() == RED)
			{
			}
			else if (v == path.begin())
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
	gv->rearrange();
}

void SystemManager::showDiscountLocations(Vertex *origin, int id, bool rent)
{
	vector<Vertex *> v = graph.discountLocations(rent, DISCOUNT_LOCATIONS);

	if (!v.size())
	{
		cout << endl
			 << " - There is no Sharing Locations on this graph." << endl;
		waitConfirm();
		return;
	}

	vector<Vertex> path = getDiscountChoice(v, origin->getInfo());

	if (!path.size())
	{
		cout << endl
			 << " - There were " << DISCOUNT_LOCATIONS << " Locations, but none of them are reachable from your current location" << endl;
		waitConfirm();
		return;
	}

	paintPath(path, true, 5, START_NODE_COLOR, RED, GREEN, PATH_COLOR);

	cout << endl
		 << " - Check the map to see your travel route!" << endl;

	cout << endl
		 << TAB << "  - Yellow Node is the given position" << endl;

	cout << endl
		 << TAB << "  - Green Nodes are the path to destiny" << endl;

	cout << endl
		 << TAB << "  - Red Node right next to Green Node is your destiny" << endl;

	cout << endl
		 << TAB;
	rent ? cout << "  - You have lifted a bike!" << endl : cout << "  - You have deposited a bike!" << endl;

	waitConfirm();

	paintPath(path, false, 1);

	rent ? ((SharingLocation *)path.back().getInfo())->liftBike() : ((SharingLocation *)path.back().getInfo())->depositBike();
}

vector<Vertex> SystemManager::getDiscountChoice(const vector<Vertex *> v, Location *origin)
{
	int userChoice;
	string input;
	auto exists = v.end();
	map<int, vector<Vertex>> paths;
	vector<pair<double, double>> distDiscountLoc;
	vector<Vertex> chosenPath;
	int size = v.size();

	//Fazer dijkstra e verificar se há path
	for (auto it : v)
	{
		clock_t begin, end;
		begin = clock();
		bool success = graph.dijkstraShortestPath(origin, it);
		end = clock();

		if (success)
		{
			gv->setVertexColor(it->getInfo()->getID(), CYAN);
			distDiscountLoc.push_back(make_pair(it->getDist(), timeDiff(begin, end)));
			paths.insert(make_pair(it->getInfo()->getID(), graph.getPath(origin, it->getInfo())));
		}
	}

	gv->rearrange();

	int pathSize = paths.size();

	if (pathSize == 0)
	{
		return chosenPath;
	}

	cout << endl
		 << " - You can get a discount if you choose one of the following locations!" << endl;
	cout << endl
		 << " - Here are their IDs: " << endl;

	//Mostrar sharingLocations com path
	for (int i = 0, j = 0; i < size; i++)
	{
		if (paths.find(v[i]->getInfo()->getID()) != paths.end())
		{
			cout << typeid(*v[i]->getInfo()).name() << endl;
			cout << "            - Sharing Location: node number " << v[i]->getInfo()->getID() << endl;
			cout << "                - Time: " << getTime(distDiscountLoc.at(j).first) << endl;
			cout << "                - Discount: " << getIncentive(distDiscountLoc.at(j).first) << " %" << endl;
			cout << "                - Found Location in: " << to_string(distDiscountLoc.at(j).second) << " seconds" << endl;
			j++;
		}
	}

	int valid = 0;

	while (exists == v.end())
	{
		if (valid > 0)
			cout << " - Invalid input..." << endl;
		cout << "\n - Enter your preference: ";
		getline(cin, input);

		if (isNumber(input))
		{
			userChoice = stoi(input);

			for (map<int, vector<Vertex>>::const_iterator iter = paths.begin(); iter != paths.end(); iter++)
			{
				if (iter->first == userChoice)
				{
					exists = v.begin();
					chosenPath = iter->second;
					break;
				}
			}
		}
		valid++;
	}

	for (auto it : v)
	{
		gv->setVertexColor(it->getInfo()->getID(), it->getInfo()->getColor());
	}

	return chosenPath;
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

bool SystemManager::checkConnectivity()
{
	clearScreen();
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
		gv->setVertexColor(id + 1, GREEN);
		gv->rearrange();
		Sleep(300);

		if (counter != vertexes.size())
		{
			end = clock();
			cout << "\n    - Failed on Node " << vertexes.at(id)->getInfo()->getID() << endl;
			cout << "\n    - See on GraphViewer" << endl;
			timeSpent = timeDiff(begin, end) - (id + 1) * 0.3;
			cout << "\n    - Time taken to check connectivity file: " << timeSpent << endl;
			cout << "\n    - Graph is not connected\n";

			waitConfirm();

			for (unsigned int i = 0; i <= id; i++)
				gv->setVertexColor(i + 1, vertexes.at(i)->getInfo()->getColor());

			gv->rearrange();

			return false;
		}
	}

	for (Vertex *v : vertexes)
	{
		gv->setVertexColor(v->getInfo()->getID(), v->getInfo()->getColor());
	}
	gv->rearrange();

	end = clock();
	timeSpent = timeDiff(begin, end);
	cout << "\n - Time taken to check connectivity file: " << timeSpent << endl
		 << endl;
	cout << " - This Graph is connected" << endl
		 << endl
		 << " - See in GraphViewer . . ." << endl;

	string quit = "";

	for (unsigned int id = 0; id < vertexes.size(); id++)
	{
		for (Vertex *v : vertexes)
		{
			v->getInfo()->setVisited(false);
		}

		if (id < vertexes.size())
		{
			if (id == 0)
				cout << "\n - Press any key to continue to see Connectivity on Vertex (" << id + 1 << ")"
					 << " or 'q' to stop ";
			else
				cout << "\n - Press any key to continue to Vertex (" << id + 1 << ")"
					 << " or 'q' to stop ";

			getline(cin, quit);

			if (quit != "q")
				Sleep(800);
			else
				break;
		}

		connectedCiclePaint(vertexes.at(id));

		Sleep(1000);
		for (Vertex *v : vertexes)
		{
			gv->setVertexColor(v->getInfo()->getID(), v->getInfo()->getColor());
			for (Edge e : v->getAdj())
			{
				gv->setEdgeColor(e.getID(), BLACK);
				gv->setEdgeThickness(e.getID(), 1);
			}
		}
		gv->rearrange();
	}

	waitConfirm();

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
	Sleep(500);
	gv->setVertexColor(v->getInfo()->getID(), GREEN);
	gv->rearrange();
	for (Edge e : v->getAdj())
	{
		if (!e.getDest()->getInfo()->getVisited())
		{
			gv->setEdgeColor(e.getID(), RED);
			gv->setEdgeThickness(e.getID(), 5);
			connectedCiclePaint(e.getDest());
		}
	}
}

void SystemManager::sharingLocationsInfo()
{
	vector<Vertex *> sharingLocations;
	vector<Vertex *> vertexSet = graph.getVertexSet();

	copy_if(vertexSet.begin(), vertexSet.end(), back_inserter(sharingLocations), [](Vertex *vertex) { return string(typeid(*vertex->getInfo()).name()) == "class SharingLocation"; });

	if (!sharingLocations.size())
	{
		cout << endl
			 << " - No Sharing Locations on this graph" << endl;

		waitConfirm();

		return;
	}

	cout << endl;

	for (unsigned int i = 0; i < sharingLocations.size(); i++)
	{
		Vertex *v = sharingLocations.at(i);
		SharingLocation *s = (SharingLocation *)v->getInfo();

		if (!i)
			cout << " _________________________________ " << endl;
		else
			cout << endl;

		cout << "| Sharing Location    " << setw(4) << i + 1 << "        |" << endl;
		cout << "|  - ID               " << setw(9) << s->getID() << "   |" << endl;
		cout << "|  - Color            " << setw(9) << s->getColor() << "   |" << endl;
		cout << "|  - Max capacity     " << setw(9) << s->getMaxCapacity() << "   |" << endl;
		cout << "|  - Number of slots  " << setw(9) << s->getSlots() << "   |" << endl;
		cout << "|  - Altitude         " << setw(9) << s->getAltitudecoord() << "   |" << endl;
		cout << "|  - Latitude         " << setw(9) << s->getLatitudecoord() << "   |" << endl;
		cout << "|  - Longitude        " << setw(9) << s->getLongitudecoord() << "   |" << endl;

		if (v->getAdj().size() != 0)
		{
			cout << "|  - Adjacent edges:              |" << endl;

			int aux = 1;
			for (Edge &e : v->getAdj())
			{
				cout << "|      - Edge " << setw(3) << aux++ << "      ID " << setw(5) << e.getID() << "   |" << endl;
			}
		}
		cout << "|_________________________________|";
	}
	waitConfirm();
}