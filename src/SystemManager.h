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

class Node;

typedef struct
{
	string nodes;
	string edges;
	string lines;

}FileNames;

class SystemManager
{
public:
	SystemManager();
	~SystemManager();


	void loadFiles();
	void selectGraph();

private:
	//GraphViewer * gv;
	Graph<Node> graph;
	FileNames fileNames;
	std::vector<Node> nodes;
	
};

#endif /* SYSTEMMANAGER_H */
