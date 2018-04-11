#include "SystemManager.h"
#include "Utils.h"
#include <string>
#include <fstream>
#include <ctime>

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

void SystemManager::initFileNames(string nodes, string edges, string names, string sharing)
{
	this->fileNames.nodes = nodes;
	this->fileNames.edges = edges;
	this->fileNames.names = names;
	this->fileNames.sharingLocations = sharing;
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

			double lat, lon, projx, projy, alt;
			char ign;
			string isShrLoc;

			read >> id >> ign >> lat >> ign >> lon >> ign >> projx >> ign >> projy >> ign >> alt >> ign;
			getline(read, isShrLoc);

			gv->addNode(idInt, (int)XCONST * convertLongitudeToX(lon), (int)YCONST * convertLatitudeToY(lat) - (int)YB);
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

void SystemManager::loadSpeficifNodes(unordered_map<int, unsigned long long> &idsNodes, const vector<SharingLoc> &sharingLocations, int option)
{
	cout << " - File: " << fileNames.nodes << endl;

	if (option == 7)
	{
		int id = 1;

		//1
		gv->addNode(id, 5, 5);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.14996, -8.614044, 95));
		id++;
		//2
		gv->addNode(id, 10, 0);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.14993, -8.614813, 94));
		id++;
		//3
		gv->addNode(id, 0, 10);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.15299, -8.612018, 113));
		id++;
		//4
		gv->addNode(id, 10, 10);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.1536, -8.611996, 113));
		id++;
		//5
		gv->addNode(id, 15, 7);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.14837, -8.61437, 95));
		id++;
		//6
		gv->addNode(id, 0, 15);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.14795, -8.612149, 89));
		id++;
		//7
		gv->addNode(id, 12, 15);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.14788, -8.611281, 73));
		id++;
		//8
		gv->addNode(id, 8, 20);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.14640, -8.611245, 89));
		id++;
	}
	else if (option == 8) // conexo
	{
		int id = 1;

		//1
		gv->addNode(id, 0, 0);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.14996, -8.614044, 95));
		id++;
		//2
		gv->addNode(id, 10, 0);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.14993, -8.614813, 94));
		id++;
		//3
		gv->addNode(id, 0, 10);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.15299, -8.612018, 113));
		id++;
		//4
		gv->addNode(id, 10, 10);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.1536, -8.611996, 113));
		id++;
		//5
		gv->addNode(id, 15, 7);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.14837, -8.61437, 95));
		id++;
		//6
		gv->addNode(id, 0, 15);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.14795, -8.612149, 89));
		id++;
		//7
		gv->addNode(id, 10, 15);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.14788, -8.611281, 73));
		id++;
		//8
		gv->addNode(id, 15, 13);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.14784, -8.610926, 73));
		id++;
		//9
		gv->addNode(id, 5, 20);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.1488, -8.612135, 89));
		id++;
		//10
		gv->addNode(id, 15, 20);
		gv->setVertexLabel(id, to_string(id));
		idsNodes.insert(make_pair(id, id));
		graph.addVertex(new Location(id, 41.14717, -8.612186, 73));
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
			string strId, strOri, strDest;
			getline(read, strId, ';');
			getline(read, strOri, ';');
			getline(read, strDest);

			id = stoull(strId);
			ori = stoull(strOri);
			dest = stoull(strDest.substr(0, strDest.size() - 1));

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
					weight = calcWeight(destiny->getInfo(), origin->getInfo());
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

unordered_map<int, unsigned long long> SystemManager::loadFiles(int option)
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
	cout << "         Time to read Sharing Locations file: " << timeSpent << " seconds" << endl << endl;

	begin = clock();
	if (option == 7 || option == 8)
		loadSpeficifNodes(idsNodes, sharingLocations, option);
	else
		loadNodes(idsNodes, sharingLocations);
	end = clock();
	timeSpent = timeDiff(begin, end);
	cout << "         Time to read Nodes file: " << timeSpent << " seconds" << endl << endl;

	begin = clock();
	edgesNames = loadNames();
	end = clock();
	timeSpent = timeDiff(begin, end);
	cout << "         Time to read Names file: " << timeSpent << " seconds" << endl << endl;

	begin = clock();
	loadEdges(edgesNames, idsNodes);
	vector<Vertex*> vertexSet = graph.getVertexSet();
	for (unsigned int i = 0; i < vertexSet.size(); i++)
	{
		if (vertexSet.at(i)->getAdj().size() > 2 && vertexSet.at(i)->getInfo()->getColor() != RED)
		{
			gv->setVertexColor(i + 1, MAGENTA);
			graph.findVertex(new Location(i + 1))->getInfo()->setColor(MAGENTA);
		}
	}
	end = clock();
	timeSpent = timeDiff(begin, end);
	cout << "         Time to read Edges file: " << timeSpent << " seconds" << endl;


	gv->rearrange();

	return idsNodes;
}