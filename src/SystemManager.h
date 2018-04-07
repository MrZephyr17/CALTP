// SystemManager.h
#pragma once

#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#ifdef _WIN32
#define Limpar_ecra() system("cls");

#else
#define Limpar_ecra() system("clear");
#endif

#include "Graph.h"
#include "Location.h"
#include "GraphViewer.h"
#include <map>
#include <unordered_map>

#define DISCOUNT_LOCATIONS 5
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

typedef struct
{
	std::string nodes;
	std::string edges;
	std::string names;
	std::string sharingLocations;

} FileNames;

struct SharingLoc
{
	unsigned long long id;
	int lotation;
	int slots;
	SharingLoc(unsigned long long i, int lot, int slot)
	{
		id = i;
		lotation = lot;
		slots = slot;
	};
};

struct EdgeName
{

	bool isBidirectional;
	string name;
	unsigned long long id;
	EdgeName(unsigned long long i, string n, bool a)
	{
		id = i;
		name = n;
		isBidirectional = a;
	};
};

class SystemManager
{
  public:
	SystemManager();
	~SystemManager();

	unordered_map<int, unsigned long long> loadFiles();
	void selectGraph();
	bool Menu();
	bool mainMenu(const unordered_map<int, unsigned long long> &idsNodes);
	bool menuRent();
	bool menuHasBike();
	bool menuSave(const unordered_map<int, unsigned long long> &idsNodes);
	void loadSharingLocations(vector<SharingLoc> &sharingLocations);
	void loadEdges(vector<EdgeName> &edges, unordered_map<int, unsigned long long> &val);
	void loadNodes(unordered_map<int, unsigned long long> &idsNodes, const vector<SharingLoc> &sharingLocations);
	vector<EdgeName> loadNames();
	void saveSharingLocations(const unordered_map<int, unsigned long long> &idsNodes);
	Vertex *findLocation(int name) const;
	int convertLongitudeToX(float longitude);
	int convertLatitudeToY(float latitude);
	void paintPath(vector<Vertex> path, bool def, int edgeThickness, string startNodeColor = "", string endNodeColor = "", string edgeColor = "BLACK");
	Vertex* getDiscountChoice(const vector<Vertex *> &v) const;
	void showClosestLocation(Vertex *origin, int id, bool rent);
	void showDiscountLocations(Vertex *origin, int id, bool rent);
	void checkConnectivity();
	bool isConnected();
	void isConnectedAux(Vertex *v, int &counter);
	void initGraphViewer();
	void initFileNames(string nodes, string edges, string names, string sharing);
	double getPathLength(const vector<Vertex> &path);

  private:
	GraphViewer *gv;
	Graph graph;
	FileNames fileNames;
};

#endif /* SYSTEMMANAGER_H */
