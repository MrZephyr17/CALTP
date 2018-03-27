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

typedef struct
{
	std::string nodes;
	std::string edges;
	std::string lines;

}FileNames;

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

private:
	//GraphViewer * gv;
	Graph<Vertex<Location>> graph;
	FileNames fileNames;
	std::vector<Vertex<Location>> nodes;

};

#endif /* SYSTEMMANAGER_H */
