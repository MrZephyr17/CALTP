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

#define DISCOUNT_LOCATIONS 5

typedef struct
{
	std::string nodes;
	std::string edges;
	std::string names;
	std::string sharingLocations;

}FileNames;

struct SharingLoc {
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

struct EdgeName {
	
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


	void loadFiles();
	void selectGraph();
	bool Menu();
	bool mainMenu();
	bool menuRent();
	bool menuHasBike();
	void loadSharingLocations(vector<SharingLoc> &sharingLocations);
	void loadEdges(vector<EdgeName> &edges, vector<pair<int, unsigned long long>> &val);
	void loadNodes(vector<pair<int, unsigned long long>> &idsNodes, const vector<SharingLoc> &sharingLocations);
	vector<EdgeName> loadNames();
	Vertex* findLocation(int name) const;
	int convertLongitudeToX(float longitude);
	int convertLatitudeToY(float latitude);
	void paintPath(vector<Vertex> path, bool def, int edgeThickness, string startNodeColor = "", string endNodeColor = "", string edgeColor = "BLACK");
	Vertex* getDiscountChoice(const vector<Vertex*> &v) const;
	void showClosestLocation(Vertex* origin, int id);
	void showDiscountLocations(Vertex* origin, int id, bool rent);


private:
	GraphViewer * gv;
	Graph graph;
	FileNames fileNames;
};

#endif /* SYSTEMMANAGER_H */
