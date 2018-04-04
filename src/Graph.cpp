#include "Graph.h"
#include <typeinfo>
#include <iostream>
#include <string>

Vertex::Vertex(Location* in) : info(in) {}

void Vertex::addEdge(Vertex *d, double w, int id, string name) {
	adj.push_back(Edge(d, w, id, name));
}

bool Vertex::operator<(Vertex & vertex) const {
	return this->dist < vertex.dist;
}


Location* Vertex::getInfo() const {
	return this->info;
}


double Vertex::getDist() const {
	return this->dist;
}


Vertex *Vertex::getPath() const {
	return this->path;
}

vector<Edge> Vertex::getAdj() const
{
	return adj;
}

void Vertex::setVisited(bool visited)
{
	this->visited = visited;
}

void Vertex::setDist(double dist)
{
	this->dist = dist;
}

void Vertex::setPath(Vertex* path)
{
	this->path = path;
}

void Vertex::setQueueIndex(int index)
{
	queueIndex = index;
}


Edge::Edge(int id, Vertex *d, double w) : id(id), dest(d), weight(w) {}

inline Edge::Edge(Vertex* d, double w, int id, string name) : id(id), dest(d), weight(w), name(name) {}

inline Edge::Edge(int id, Vertex* d) : id(id), dest(d) {}

inline Edge::Edge(int id) : id(id) {}

int Edge::getID()
{
	return id;
}

int Graph::getNumVertex() const {
	return vertexSet.size();
}


vector<Vertex *> Graph::getVertexSet() const {
	return vertexSet;
}

Vertex * Graph::findVertex(Location* in) const {
	for (auto v : vertexSet)
		if (*v->info == *in)
			return v;
	return NULL;
}

Edge Graph::findEdge(Location* org, const Location* dest) const {

	Vertex* origin = findVertex(org);
	
	for (auto e : origin->getAdj())
	{
		if (e.dest->getInfo() == dest)
			return e;
	}

	return Edge(-1);
}

bool Graph::addVertex(Location* in) {
	if (findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex(in));
	return true;
}

bool Graph::addEdge(Location* sourc, Location* dest, double w, int id, string name) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2, w, id, name);
	return true;
}

/**************** Single Source Shortest Path algorithms ************/

Location* Graph::dijkstraShortestPath(Location* origin) {
	// TODO
	for (Vertex* v : vertexSet)
	{
		v->dist = DBL_MAX;
		v->path = NULL;
	}

	Vertex* vertex = findVertex(origin);
	vertex->dist = 0;

	MutablePriorityQueue<Vertex > queue;
	queue.insert(vertex);

	Vertex *min= nullptr;
	string color = "BLUE";

	while (!queue.empty() && (min == nullptr || !min->info->isAvailable()) &&  (color == "BLUE" ||min->getInfo() == origin))
	{
		min = queue.extractMin();
		color = min->getInfo()->getColor();


		for (Edge w : min->adj)
		{
			Vertex *v2 = w.dest;
			if (v2->dist > min->dist + w.weight)
			{
				double oldDist = v2->dist;
				v2->dist = min->dist + w.weight;
				v2->path = min;
				if (oldDist == DBL_MAX)
					queue.insert(v2);
				else
					queue.decreaseKey(v2);
			}
		}
	}

	return min->getInfo();
}

vector<Vertex> Graph::getPath(Location* origin, Location* dest) const {
	vector<Vertex> res, res_aux;

	Vertex* o = findVertex(origin);
	Vertex* d = findVertex(dest);

	while (d != o)
	{
		res_aux.push_back(*d);
		d = d->path;
	}

	res_aux.push_back(d->info);

	for (int i = res_aux.size() - 1; i >= 0; i--)
	{
		res.push_back(res_aux.at(i));
	}

	return res;
}

vector<Vertex*> Graph:: highestLocations()
{
	vector<Vertex*> sharingLocations;
	
	copy_if(vertexSet.begin(), vertexSet.end(), back_inserter(sharingLocations), [](Vertex* vertex) {
		return strcmp(typeid(*vertex->getInfo()).name(), "class SharingLocation") == 0; });

	/*for (auto vertex : vertexSet)
	{
		cout << "|" << typeid(*vertex->getInfo()).name() << "|\n";
		if (strcmp(typeid(*vertex->getInfo()).name(),"class SharingLocation") == 0)
		{
			sharingLocations.push_back(vertex);
			cout << "Oi\n";
		}
	}*/

	cout << sharingLocations.size() << endl;

	sort(sharingLocations.begin(), sharingLocations.end(), [](const Vertex* lhs, const Vertex* rhs) {
		return lhs->getInfo()->getAltitudecoord() > rhs->getInfo()->getAltitudecoord();
	});

	sharingLocations.resize(5);
	return sharingLocations;
}
