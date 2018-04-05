#include "Graph.h"
#include <typeinfo>
#include <iostream>
#include <string>
#include <iterator>

Vertex::Vertex(Location* in) : info(in) {}

Vertex::~Vertex()
{
	delete info;
}

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

Edge::Edge(int id, Vertex *d, double w) : id(id), dest(d), weight(w) {}

Edge::~Edge()
{
	delete dest;
}

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


Graph::~Graph()
{
	for (auto it = vertexSet.begin(); it!= vertexSet.end(); it++)
		delete *it;

	vertexSet.clear();
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

Location* Graph::dijkstraShortestPath(Location* origin) 
{
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

void Graph::bidirectionalSearch(Location * origin, Location * destiny, Graph &invGraph)
{
	for (Vertex* v : vertexSet)
	{
		v->dist = DBL_MAX;
		v->path = NULL;
	}

	Vertex* src = findVertex(origin);
	Vertex* dest = invGraph.findVertex(destiny);
	src->dist = 0;
	dest->dist = 0;

	MutablePriorityQueue<Vertex> queueSrc;
	MutablePriorityQueue<Vertex> queueDest;
	src->getInfo()->setVisited(true);
	queueSrc.insert(src);
	dest->getInfo()->setVisited(true);
	queueSrc.insert(dest);

	Vertex *x1 = nullptr;
	Vertex *x2 = nullptr;

	while (!queueSrc.empty() && !queueDest.empty())
	{
		if (!queueSrc.empty())
		{
			x1 = queueSrc.extractMin();

			if (x1->getInfo()->getVisited())
				return;

			for (Edge w : x1->adj)
			{
				Vertex *v2 = w.dest;
				if (v2->dist > x1->dist + w.weight)
				{
					double oldDist = v2->dist;
					v2->dist = x1->dist + w.weight;
					v2->path = x1;
					if (oldDist == DBL_MAX)
						queueSrc.insert(v2);
					else
						queueSrc.decreaseKey(v2);
				}
			}
		}

		if (!queueDest.empty())
		{
			x2 = queueDest.extractMin();

			if (x2->getInfo()->getVisited())
				return;

			for (Edge w : x2->adj)
			{
				Vertex *v2 = w.dest;
				if (v2->dist > x2->dist + w.weight)
				{
					double oldDist = v2->dist;
					v2->dist = x2->dist + w.weight;
					v2->path = x2;
					if (oldDist == DBL_MAX)
						queueDest.insert(v2);
					else
						queueDest.decreaseKey(v2);
				}
			}
		}


	}
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

vector<Vertex*> Graph:: discountLocations(bool rent, const int numberOfLocations)
{
	vector<Vertex*> sharingLocations;
	
	copy_if(vertexSet.begin(), vertexSet.end(), back_inserter(sharingLocations), [](Vertex* vertex) {
		return strcmp(typeid(*vertex->getInfo()).name(), "class SharingLocation") == 0; });

		sort(sharingLocations.begin(), sharingLocations.end(), [rent](const Vertex* lhs, const Vertex* rhs) {
			return rent ? ((SharingLocation*)lhs->getInfo())->getSlots() > ((SharingLocation*)lhs->getInfo())->getSlots() :
				((SharingLocation*)lhs->getInfo())->getSlots() < ((SharingLocation*)lhs->getInfo())->getSlots();
		});

	sharingLocations.resize(numberOfLocations);
	return sharingLocations;
}
