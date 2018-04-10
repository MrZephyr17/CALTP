// SystemManager.h
#pragma once

#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "Graph.h"
#include "Location.h"
#include "GraphViewer.h"
#include <map>
#include <unordered_map>

#ifdef _WIN32
#define clearScreen() system("cls");

#else
#define clearScreen() system("clear");
#endif

/** @defgroup systemmanager systemmanager
 * @{
 *
 * Various functions and constants to support various features of the program. 
 */

#define DISCOUNT_LOCATIONS 5
#define EDGE_COLOR_DEFAULT BLACK
#define VERTEX_COLOR_DEFAULT BLUE
#define START_NODE_COLOR YELLOW
#define END_NODE_COLOR WHITE
#define PATH_COLOR MAGENTA
#define TAB "      "

/** @name FileNames */
/**@{
 *
 * Struct to store information about entry file names
 */
typedef struct
{
	std::string nodes;
	std::string edges;
	std::string names;
	std::string sharingLocations;

} FileNames;

/** @} end of FileNames*/

/** @name SharingLoc */
/**@{
 *
 * Struct to temporarilly store information about a sharing location
 */
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

/** @} end of SharingLoc*/

/** @name EdgeName */
/**@{
 *
 * Struct to temporarilly store information about an edge
 */
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

/** @} end of EdgeName*/

/**
 * @brief 
 * 
 */
class SystemManager
{
  public:
	/**
   * @brief Construct a new System Manager object
   * 
   */
	SystemManager();
	~SystemManager();

	/**
	 * @brief 
	 * 
	 * @return unordered_map<int, unsigned long long> 
	 */
	unordered_map<int, unsigned long long> loadFiles();
	/**
	 * @brief 
	 * 
	 */
	void selectGraph();
	/**
	 * @brief 
	 * 
	 * @return true 
	 * @return false 
	 */
	bool Menu();
	/**
	 * @brief 
	 * 
	 * @param idsNodes 
	 * @return true 
	 * @return false 
	 */
	bool mainMenu(const unordered_map<int, unsigned long long> &idsNodes);
	/**
	 * @brief 
	 * 
	 * @return true 
	 * @return false 
	 */
	bool menuRent();
	/**
	 * @brief 
	 * 
	 * @return true 
	 * @return false 
	 */
	bool menuHasBike();
	/**
	 * @brief 
	 * 
	 * @param idsNodes 
	 * @return true 
	 * @return false 
	 */
	bool menuSave(const unordered_map<int, unsigned long long> &idsNodes);
	/**
	 * @brief 
	 * 
	 * @param sharingLocations 
	 */
	void loadSharingLocations(vector<SharingLoc> &sharingLocations);
	/**
	 * @brief 
	 * 
	 * @param edges 
	 * @param val 
	 */
	void loadEdges(vector<EdgeName> &edges, unordered_map<int, unsigned long long> &val);
	/**
	 * @brief 
	 * 
	 * @param idsNodes 
	 * @param sharingLocations 
	 */
	void loadNodes(unordered_map<int, unsigned long long> &idsNodes, const vector<SharingLoc> &sharingLocations);
	/**
	 * @brief 
	 * 
	 * @return vector<EdgeName> 
	 */
	vector<EdgeName> loadNames();
	/**
	 * @brief 
	 * 
	 * @param idsNodes 
	 */
	void saveSharingLocations(const unordered_map<int, unsigned long long> &idsNodes);

	/**
	 * @brief 
	 * 
	 * @param path 
	 * @param def 
	 * @param edgeThickness 
	 * @param startNodeColor 
	 * @param endNodeColor 
	 * @param elseNodeColor 
	 * @param edgeColor 
	 */
	void paintPath(vector<Vertex> path, bool def, int edgeThickness, string startNodeColor = "", string endNodeColor = "", string elseNodeColor = "", string edgeColor = "BLACK");

	/**
	 * @brief Get the Discount Choice object
	 * 
	 * @param v 
	 * @param origin 
	 * @return vector<Vertex> 
	 */
	vector<Vertex> getDiscountChoice(const vector<Vertex *> v, Location *origin);
	/**
	 * @brief 
	 * 
	 * @param origin 
	 * @param id 
	 * @param rent 
	 */
	void showClosestLocation(Vertex *origin, int id, bool rent);
	/**
	 * @brief 
	 * 
	 * @param origin 
	 * @param id 
	 * @param rent 
	 */
	void showDiscountLocations(Vertex *origin, int id, bool rent);
	/**
	 * @brief 
	 * 
	 * @return true 
	 * @return false 
	 */
	bool checkConnectivity();
	/**
	 * @brief 
	 * 
	 * @param v 
	 * @param counter 
	 */
	void isConnectedAux(Vertex *v, int &counter);
	/**
	 * @brief 
	 * 
	 * @param v 
	 */
	void connectedCiclePaint(Vertex *v);
	/**
	 * @brief 
	 * 
	 */
	void initGraphViewer();
	/**
	 * @brief 
	 * 
	 * @param nodes 
	 * @param edges 
	 * @param names 
	 * @param sharing 
	 */
	void initFileNames(string nodes, string edges, string names, string sharing);
	/**
	 * @brief 
	 * 
	 */
	void sharingLocationsInfo();

  private:
	GraphViewer *gv;
	Graph graph;
	FileNames fileNames;
};

/** @} end of systemmanager */

#endif /* SYSTEMMANAGER_H */
