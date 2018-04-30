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
 * Various functions and constants to manage information for this program. 
 */

#define DISCOUNT_LOCATIONS 5	  /**< @brief Maximum number of discount locations */
#define EDGE_COLOR_DEFAULT BLACK  /**< @brief Edge's default color */
#define VERTEX_COLOR_DEFAULT BLUE /**< @brief Node's default color */
#define START_NODE_COLOR YELLOW   /**< @brief Color for the start node */
#define END_NODE_COLOR WHITE	  /**< @brief Color for the destination node */
#define PATH_COLOR MAGENTA		  /**< @brief Color for a path between two locations */
#define TAB "      "			  /**< @brief TAB space */

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

	/**
	 * @brief Destroy the System Manager object
	 */
	~SystemManager();

	/**
	 * @brief Loads entry files onto the graph and the graphviewer
	 * 
	 * @param option graph option
	 * 
	 * @return a structure associating nodes id's on the file and on the program 
	 */
	unordered_map<int, unsigned long long> loadFiles(int option);

	/**
	 * @brief Allows the user to select a graph
	 * 
	 * @return the user's choice 
	 */
	int selectGraph();

	/**
	 * @brief Selects graph, loads files and opens the main menu
	 * 
	 * @return true 
	 */
	bool Menu();
	/**
	 * @brief The program's main menu, grouping all the important features.
	 * 
	 * @param idsNodes a structure associating nodes id's on the file and on the program 
	 * @return true while the user doesn't decide to leave the program
	 */
	bool mainMenu(const unordered_map<int, unsigned long long> &idsNodes);
	/**
	 * @brief Presents options related to renting bikes
	 * 
	 * @return true if successful, false otherwise
	 */
	bool menuRent();
	/**
	 * @brief Presents options related to depositing bikes
	 * 
	 * @return true if successful, false otherwise
	 */
	bool menuHasBike();
	/**
	 * @brief Allows the user to save (or not) the graph state
	 * 
	 * @param idsNodes a structure associating nodes id's on the file and on the program 
	 * @return true if successful
	 */
	bool menuSave(const unordered_map<int, unsigned long long> &idsNodes);
	/**
	 * @brief Loads Sharing Location's onto sharingLocations
	 * 
	 * @param sharingLocations vector to be stored with the information
	 */
	void loadSharingLocations(vector<SharingLoc> &sharingLocations);
	/**
	 * @brief Loads edges onto the graph and graphviewer
	 * 
	 * @param edges stores temporary information on the edges
	 * @param val a structure associating nodes id's on the file and on the program 
	 */
	void loadEdges(vector<EdgeName> &edges, unordered_map<int, unsigned long long> &val);
	/**
	 * @brief Loads nodes onto the graph and graphviewer
	 * 
	 * @param idsNodes a structure associating nodes id's on the file and on the program 
	 * @param sharingLocations a vector containing information on sharing locations
	 */
	void loadNodes(unordered_map<int, unsigned long long> &idsNodes, const vector<SharingLoc> &sharingLocations);

	/**
	 * @brief Loads Nodes related to graph connectivity testing
	 * 
	 * @param idsNodes a structure associating nodes id's on the file and on the program 
	 * @param sharingLocations a vector containing information on sharing locations
	 * @param option the user's choice
	 */
	void loadSpeficicNodes(unordered_map<int, unsigned long long> &idsNodes, const vector<SharingLoc> &sharingLocations, int option);

	/**
	 * @brief Loads the names file
	 * 
	 * @return information related to edges
	 */
	vector<EdgeName> loadNames();

	/**
	 * @brief Saves sharing locations to their respective file
	 * 
	 * @param idsNodes a structure associating nodes id's on the file and on the program 
	 */
	void saveSharingLocations(const unordered_map<int, unsigned long long> &idsNodes);

	/**
	 * @brief Paints a path on the graph viewer
	 * 
	 * @param path the successive nodes composing the path
	 * @param def paint or erase the path
	 * @param edgeThickness the edge's thickness
	 * @param startNodeColor the origin node's color
	 * @param endNodeColor the destination node's color
	 * @param elseNodeColor the remaining edge's color
	 * @param edgeColor the edge's color
	 */
	void paintPath(vector<Vertex> path, bool def, int edgeThickness, string startNodeColor = "", string endNodeColor = "", string elseNodeColor = "", string edgeColor = "BLACK");

	/**
	 * @brief Get the user's discount path choice
	 * 
	 * @param v The location's available on the discount system
	 * @param origin the start location
	 * @return the user's selected path 
	 */
	vector<Vertex> getDiscountChoice(const vector<Vertex *> v, Location *origin);
	/**
	 * @brief Shows the closest location (if existant) to the user, and its path
	 * 
	 * @param origin The user's location
	 * @param id the location's id
	 * @param rent indicates if the user wants to rent or deposit
	 */
	void showClosestLocation(Vertex *origin, int id, bool rent);
	/**
	 * @brief Shows the path to the discount location choses by the user
	 * 
	 * @param origin The user's location
	 * @param id the location's id
	 * @param rent indicates if the user wants to rent or deposit
	 */
	void showDiscountLocations(Vertex *origin, int id, bool rent);
	/**
	 * @brief Checks connectivity for a graph
	 * 
	 * @return true if connected, false otherwise
	 */
	bool checkConnectivity();
	/**
	 * @brief Auxilairy function to check if a graph is connected
	 * 
	 * @param v origin vertex
	 * @param counter counts the number of vertexes that can be reached by v
	 */
	void isConnectedAux(Vertex *v, int &counter);
	/**
	 * @brief Paints on GraphViewer a connected cicle starting on vertex v
	 * 
	 * @param v origin vertex
	 */
	void connectedCiclePaint(Vertex *v);
	/**
	 * @brief Initializes GraphViewer
	 */
	void initGraphViewer();
	/**
	 * @brief Initializes the names of the files to be read
	 * 
	 * @param nodes the node's file name
	 * @param edges the edges' file name
	 * @param names the names' file name
	 * @param sharing the sharing locations' file name
	 */
	void initFileNames(string nodes, string edges, string names, string sharing);

	/**
	 * @brief Prints information on the sharing locations
	 */
	void sharingLocationsInfo();

	void findSLExact(std::street1, std::street2);

	void findSLApproximate(std::street1, std::street2);

  private:
	GraphViewer *gv;
	Graph graph;
	FileNames fileNames;
};

/** @} end of systemmanager */

#endif /* SYSTEMMANAGER_H */
