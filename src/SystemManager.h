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

typedef struct
{
	std::string nodes;
	std::string edges;
	std::string names;

}FileNames;

struct EdgeInfo
{
	Location* origin;
	Location* dest;
	unsigned long long id;
	EdgeInfo(unsigned long long  i, Location* o, Location* d)
	{
		id = i;
		origin = o;
		dest = d;
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
	void loadEdges(vector<EdgeInfo> &edges, string edge);
	void loadNodes(string node);
	void loadNames(vector<EdgeInfo> edges, string name);
	Vertex* findLocation(string name) const;

private:
	GraphViewer * gv;
	Graph graph;
	FileNames fileNames;
};

#endif /* SYSTEMMANAGER_H */
