/*
* Graph.h
*/
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include "MutablePriorityQueue.h"
#include "Location.h"

using namespace std;

class Edge;
class Graph;
class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

class Vertex {
	Location info;                // contents
	vector<Edge> adj;  // outgoing edges
	bool visited;          // auxiliary field
	double dist = 0;
	Vertex *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue

	bool processing = false;

	/*
	* Auxiliary function to add an outgoing edge to a vertex (this),
	* with a given destination vertex (d) and edge weight (w).
	*/
	void addEdge(Vertex *dest, double w, int id, string name);

public:
	Vertex(Location in);
	bool operator<(Vertex & vertex) const; // // required by MutablePriorityQueue
	Location getInfo() const;
	double getDist() const;
	Vertex *getPath() const;
	friend class Graph;
	friend class MutablePriorityQueue<Vertex>;
};








/********************** Edge  ****************************/


class Edge {
	int id;
	Vertex * dest;      // destination vertex
	double weight;         // edge weight
	string name;           // edge name
public:
	Edge(int id, Vertex *d, double w);
	Edge(Vertex *d, double w, int id, string name);
	Edge(int id, Vertex *d);
	friend class Graph;
	friend class Vertex;
};




/*************************** Graph  **************************/


class Graph {
	vector<Vertex*> vertexSet;    // vertex set

public:
	/*
	* Auxiliary function to find a vertex with a given content.
	*/
	Vertex *findVertex(const Location &in) const;
	/*
	*  Adds a vertex with a given content or info (in) to a graph (this).
	*  Returns true if successful, and false if a vertex with that content already exists.
	*/
	bool addVertex(const Location &in);
	/*
	* Adds an edge to a graph (this), given the contents of the source and
	* destination vertices and the edge weight (w).
	* Returns true if successful, and false if the source or destination vertex does not exist.
	*/
	bool addEdge(const Location &sourc, const Location &dest, double w, int id, string name);
	int getNumVertex() const;
	vector<Vertex *> getVertexSet() const;

	// Fp05 - single source
	Location dijkstraShortestPath(const Location &s);
	Location dijkstraShortestPathOld(const Location &s);
	vector<Vertex> getPath(const Location &origin, const Location &dest) const;

};

#endif /* GRAPH_H_ */
