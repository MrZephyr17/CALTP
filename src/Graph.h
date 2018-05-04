#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <string>
#include <map>
#include <utility>
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
 * @brief Represents a Vertex on a Graph
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

	/**
	 * @brief Auxiliary function to add an outgoing edge to a vertex (this)
	 * 
	 * @param dest destination vertex
	 * @param w edge weight
	 * @param id destination vertex's id
	 * @param edge's name 
	 */
	void addEdge(Vertex *dest, double w, int id, std::string name);

  public:
	/**
	 * @brief Construct a new Vertex object
	 * 
	 * @param in this vertex's location
	 */
	Vertex(Location *in);

	/**
	 * @brief Destroy the Vertex object
	 */
	~Vertex();
	/**
	 * @brief Checks if a vertex has less dist than the new
	 * 
	 * @param vertex the vertex to be compared with
	 * @return true if lower, false otherwise
	 */
	bool operator<(Vertex &vertex) const; // // required by MutablePriorityQueue

	/**
	 * @brief Get this Vertex's location
	 * 
	 * @return the associated location 
	 */
	Location *getInfo() const;

	/**
	 * @brief Get the distance
	 * 
	 * @return the distance 
	 */
	double getDist() const;

	/**
	 * @brief Gets the destination, set on Dijkstra
	 * 
	 * @return the destination vertex
	 */
	Vertex *getPath() const;

	/**
	 * @brief Get the adjacent edges
	 * 
	 * @return the adjacent edges
	 */
	vector<Edge> getAdj() const;

	friend class Graph;
	friend class MutablePriorityQueue<Vertex>;
};

/********************** Edge  ****************************/

/**
 * @brief Represents an Edge on the Graph 
 */
class Edge
{
	int id;
	Vertex *dest;	 // destination vertex
	double weight;	// edge weight
	std::string name; // edge name
  public:
	/**
	 * @brief Construct a new Edge object with destination, id and weight
	 * 
	 * @param id This edge's id
	 * @param d This edge's destination
	 * @param w This edge's weight
	 */
	Edge(int id, Vertex *d, double w);

	Edge();

	/**
	 * @brief Destroy the Edge object
	 */
	~Edge();

	/**
	 * @brief Construct a new Edge object width destination, id and name
	 * 
	 * @param d This edge's destination
	 * @param w This edge's weight
	 * @param id This edge's id
	 * @param name This edge's name
	 */
	Edge(Vertex *d, double w, int id, std::string name);

	/**
	 * @brief Construct a new Edge object with a destination
	 * 
	 * @param id this edge's id
	 * @param d this edge's destination
	 */
	Edge(int id, Vertex *d);

	/**
	 * @brief Construct a new Edge object
	 * 
	 * @param id this edge's id
	 */
	Edge(int id);

	/**
	 * @brief Gets this edge id
	 * 
	 * @return this edge id 
	 */
	int getID();

	/**
	 * @brief Get the destination vertex
	 * 
	 * @return the destination vertex
	 */
	Vertex *getDest();

	friend class Graph;
	friend class Vertex;
};

/*************************** Graph  **************************/

/**
 * @brief Represents a Graph of Locations
 */
class Graph
{
	vector<Vertex *> vertexSet; // vertex set

  public:
	/**
	 * @brief Destroy the Graph object
	 */
	~Graph();

	/**
	 * @brief Auxiliary function to find a vertex with a given content.
	 * 
	 * @param in The vertex's location
	 * @return the vertex with the given location
	 */
	Vertex *findVertex(Location *in) const;

	/**
	 * @brief Finds an edge connecting given locations
	 * 
	 * @param org The origin location
	 * @param dest The destination location
	 * @return the edge connecting the locations, with ID(-1) if non existant 
	 */
	Edge findEdge(Location *org, const Location *dest) const;

	/**
	 * @brief Adds a vertex with a given content or info (in) to a graph (this).
	 * 
	 * @param in the vertex's location
	 * @return true if successful, and false if a vertex with in already exists. 
	 */
	bool addVertex(Location *in);

	/**
	 * Adds an edge to a graph (this), given the contents of the source and
	 * destination vertices and the edge weight (w).
	 * 
	 * @param sourc the source vertex
	 * @param dest the destination vertex
	 * @param w the edge's weights
	 * @param id the edge's id
	 * @param name the edge's name
	 * 
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
	 * @brief Get the Vertex Set 
	 * 
	 * @return the vertexes
	 */
	vector<Vertex *> getVertexSet() const;

	/**
	 * @brief Calculates the shortest path between the origin location and the destination location
	 * 
	 * @param s The origin location
	 * @param destiny The destination location
	 * @return true if a path exists, false otherwise
	 */
	bool dijkstraShortestPath(Location *s, Vertex *destiny);

	/**
	 * @brief Calculates the shortest path between the origin location and the closest
	 * available sharing location
	 * 
	 * @param s the origin location
	 * @param destiny the destination location (if existant)
	 * @param rent indicates if the user wants to rent or deposit
	 * @return true if a path exists, false otherwise
	 */
	bool dijkstraShortestPath(Location *s, Vertex *&destiny, bool rent);

	/**
	 * @brief Get the path between the given locations
	 * 
	 * @param origin the origin location
	 * @param dest the destination location
	 * @return the path between the locations 
	 */
	vector<Vertex> getPath(Location *origin, Location *dest) const;

	/**
	 * @brief Returns the locations available for the discount feature
	 * 
	 * @param rent indicates if the user wants to rent or deposit
	 * @param numberOfLocations number of Locations to be returned
	 * @return the locations available for the discount feature
	 */
	vector<Vertex *> discountLocations(bool rent, const int numberOfLocations);

	/**
	 * @brief Finds a location with the given ID
	 * 
	 * @param ID The location's ID
	 * @return Vertex* the location's vertex
	 */
	Vertex *findLocation(const int ID) const;

	bool findSLExact(std::string street1, std::string street2, Vertex *location);

	std::multimap<int, std::string> findSLApproximate(std::string street1, std::string street2);
};

/** @} end of graph */

#endif /* GRAPH_H_ */
