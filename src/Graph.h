#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <string>
#include "MutablePriorityQueue.h"
#include "Location.h"
#include "SharingLocation.h"

/** @defgroup graph graph
 * @{
 *
 * Classes to represent a graph storing locations, sharing locations, and the paths between them.
 */

class Edge;
class Graph;
class Vertex;

#define INF std::numeric_limits<double>::max() /**< @brief double's max value */

/************************* Vertex  **************************/

/**
 * @brief 
 * 
 */
class Vertex
{
	Location *info;   // contents
	vector<Edge> adj; // outgoing edges
	bool visited;	 // auxiliary field
	double dist = 0;
	Vertex *path = NULL;
	int queueIndex = 0; // required by MutablePriorityQueue

	bool processing = false;

	/*
	* Auxiliary function to add an outgoing edge to a vertex (this),
	* with a given destination vertex (d) and edge weight (w).
	*/
	void addEdge(Vertex *dest, double w, int id, std::string name);

  public:
	Vertex(Location *in);
	~Vertex();
	bool operator<(Vertex &vertex) const; // // required by MutablePriorityQueue
	Location *getInfo() const;
	double getDist() const;
	Vertex *getPath() const;
	vector<Edge> getAdj() const;

	friend class Graph;
	friend class MutablePriorityQueue<Vertex>;
};

/********************** Edge  ****************************/

/**
 * @brief 
 * 
 */
class Edge
{
	int id;
	Vertex *dest;	 // destination vertex
	double weight;	// edge weight
	std::string name; // edge name
  public:
	Edge(int id, Vertex *d, double w);
	~Edge();
	Edge(Vertex *d, double w, int id, std::string name);
	Edge(int id, Vertex *d);
	Edge(int id);
	int getID();
	Vertex *getDest();
	friend class Graph;
	friend class Vertex;
};

/*************************** Graph  **************************/

/**
 * @brief 
 * 
 */
class Graph
{
	vector<Vertex *> vertexSet; // vertex set

  public:
	~Graph();

	/**
	 * @brief Auxiliary function to find a vertex with a given content.
	 * 
	 * @param in 
	 * @return Vertex* 
	 */
	Vertex *findVertex(Location *in) const;

	/**
	 * @brief 
	 * 
	 * @param org 
	 * @param dest 
	 * @return Edge 
	 */
	Edge findEdge(Location *org, const Location *dest) const;
	/*

	*/

	/**
	 * @brief Adds a vertex with a given content or info (in) to a graph (this).
	 * 
	 * @param in 
	 * @return true 
	 * @return true if successful, and false if a vertex with in already exists. 
	 */
	bool addVertex(Location *in);
	/*
	*
	*/

	/**
	 * Adds an edge to a graph (this), given the contents of the source and
	 * destination vertices and the edge weight (w).
	 * 
	 * @param sourc 
	 * @param dest 
	 * @param w 
	 * @param id 
	 * @param name 
	 * @return true if successful, and false if the source or destination vertex does not exist. 
	 */
	bool addEdge(Location *sourc, Location *dest, double w, int id, std::string name);

	/**
	 * @brief Gets the number of vertexes
	 * 
	 * @return that number 
	 */
	int getNumVertex() const;

	/**
	 * @brief Get the Vertex Set object
	 * 
	 * @return vector<Vertex *> 
	 */
	vector<Vertex *> getVertexSet() const;

	/**
	 * @brief 
	 * 
	 * @param s 
	 * @param destiny 
	 * @return true 
	 * @return false 
	 */
	bool dijkstraShortestPath(Location *s, Vertex *destiny);

	/**
	 * @brief 
	 * 
	 * @param s 
	 * @param destiny 
	 * @param rent 
	 * @return true 
	 * @return false 
	 */
	bool dijkstraShortestPath(Location *s, Vertex *&destiny, bool rent);

	/**
	 * @brief Get the Path object
	 * 
	 * @param origin 
	 * @param dest 
	 * @return vector<Vertex> 
	 */
	vector<Vertex> getPath(Location *origin, Location *dest) const;

	/**
	 * @brief 
	 * 
	 * @param rent 
	 * @param numberOfLocations 
	 * @return vector<Vertex *> 
	 */
	vector<Vertex *> discountLocations(bool rent, const int numberOfLocations);

	/**
	 * @brief 
	 * 
	 * @param ID 
	 * @return Vertex* 
	 */
	Vertex *findLocation(const int ID) const;
};

/** @} end of utils */

#endif /* GRAPH_H_ */
