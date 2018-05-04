#include "Graph.h"
#include "Utils.h"
#include "Exceptions.h"
#include <typeinfo>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>

Vertex::Vertex(Location *in) : info(in) {}

Vertex::~Vertex()
{
}

void Vertex::addEdge(Vertex *d, double w, int id, string name)
{
	adj.push_back(Edge(d, w, id, name));
}

bool Vertex::operator<(Vertex &vertex) const
{
	return this->dist < vertex.dist;
}

Location *Vertex::getInfo() const
{
	return this->info;
}

double Vertex::getDist() const
{
	return this->dist;
}

Vertex *Vertex::getPath() const
{
	return this->path;
}

vector<Edge> Vertex::getAdj() const
{
	return adj;
}

Edge::Edge(int id, Vertex *d, double w) : id(id), dest(d), weight(w) {}

Edge::Edge()
{
	id = 0;
	dest = nullptr;
	weight = 0;
}

Edge::~Edge()
{
}

inline Edge::Edge(Vertex *d, double w, int id, string name) : id(id), dest(d), weight(w), name(name) {}

inline Edge::Edge(int id, Vertex *d) : id(id), dest(d) {}

inline Edge::Edge(int id) : id(id) {}

int Edge::getID()
{
	return id;
}

Vertex *Edge::getDest()
{
	return dest;
}

int Graph::getNumVertex() const
{
	return vertexSet.size();
}

vector<Vertex *> Graph::getVertexSet() const
{
	return vertexSet;
}

Graph::~Graph()
{
	for (auto it = vertexSet.begin(); it != vertexSet.end(); it++)
		delete *it;

	vertexSet.clear();
}

Vertex *Graph::findVertex(Location *in) const
{
	auto it = find_if(vertexSet.begin(), vertexSet.end(), [in](auto v) {
		return *v->getInfo() == *in;
	});

	return it != vertexSet.end() ? *it : NULL;
}

Edge Graph::findEdge(Location *org, const Location *dest) const
{
	Vertex *origin = findVertex(org);
	vector<Edge> adj = origin->getAdj();

	auto it = find_if(adj.begin(), adj.end(), [&dest](auto e) {
		return e.dest->getInfo() == dest;
	});

	return it != adj.end() ? *it : Edge(-1);
}

bool Graph::addVertex(Location *in)
{
	if (findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex(in));
	return true;
}

bool Graph::addEdge(Location *sourc, Location *dest, double w, int id, string name)
{
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2, w, id, name);
	return true;
}

/**************** Single Source Shortest Path algorithms ************/

bool Graph::dijkstraShortestPath(Location *origin, Vertex *&dest, bool rent)
{
	for (Vertex *v : vertexSet)
	{
		v->dist = INF;
		v->path = NULL;
	}

	Vertex *vertex = findVertex(origin);
	vertex->dist = 0;

	MutablePriorityQueue<Vertex> queue;
	queue.insert(vertex);

	Vertex *min = nullptr;

	while (!queue.empty() && (min == nullptr || !min->info->isAvailable(rent) || min->getInfo() == origin))
	{
		min = queue.extractMin();

		for (Edge w : min->adj)
		{
			Vertex *v2 = w.dest;
			if (v2->dist > min->dist + w.weight)
			{
				double oldDist = v2->dist;
				v2->dist = min->dist + w.weight;
				v2->path = min;
				if (oldDist == INF)
					queue.insert(v2);
				else
					queue.decreaseKey(v2);
			}
		}
	}

	dest = min;

	return min->getInfo()->isAvailable(rent);
}

bool Graph::dijkstraShortestPath(Location *origin, Vertex *destiny)
{
	for (Vertex *v : vertexSet)
	{
		v->dist = INF;
		v->path = NULL;
	}

	Vertex *vertex = findVertex(origin);
	vertex->dist = 0;

	MutablePriorityQueue<Vertex> queue;
	queue.insert(vertex);

	Vertex *min = nullptr;
	string color = "BLUE";

	while (!queue.empty() && (min == nullptr || min->info != destiny->info))
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
				if (oldDist == INF)
					queue.insert(v2);
				else
					queue.decreaseKey(v2);
			}
		}
	}

	return min->info == destiny->info;
}

vector<Vertex> Graph::getPath(Location *origin, Location *dest) const
{
	vector<Vertex> res, res_aux;

	Vertex *o = findVertex(origin);
	Vertex *d = findVertex(dest);

	while (d != o)
	{
		res_aux.push_back(*d);
		d = d->path;
	}

	res_aux.push_back(*d);

	for (int i = res_aux.size() - 1; i >= 0; i--)
	{
		res.push_back(res_aux.at(i));
	}

	return res;
}

vector<Vertex *> Graph::discountLocations(bool rent, const int numberOfLocations)
{
	vector<Vertex *> sharingLocations;

	copy_if(vertexSet.begin(), vertexSet.end(), back_inserter(sharingLocations), [&rent](Vertex *vertex) { return string(typeid(*vertex->getInfo()).name()) == "class SharingLocation" && vertex->getInfo()->isAvailable(rent); });

	sort(sharingLocations.begin(), sharingLocations.end(), [rent](const Vertex *lhs, const Vertex *rhs) {
		return rent ? ((SharingLocation *)lhs->getInfo())->getSlots() > ((SharingLocation *)lhs->getInfo())->getSlots() : ((SharingLocation *)lhs->getInfo())->getSlots() < ((SharingLocation *)lhs->getInfo())->getSlots();
	});

	if ((int)sharingLocations.size() > numberOfLocations)
		sharingLocations.resize(numberOfLocations);

	return sharingLocations;
}

Vertex *Graph::findLocation(const int ID) const
{
	auto it = find_if(vertexSet.begin(), vertexSet.end(), [ID](Vertex *v) {
		return v->getInfo()->getID() == ID;
	});

	if (it != vertexSet.end())
		return *it;

	throw LocationNotFound(ID);
}

bool Graph::findSLExact(string street1, string street2, Vertex *location)
{
	Edge e1, e2;
	Vertex *v1 = nullptr, *v2 = nullptr;

	for (auto x : vertexSet)
	{
		for (auto y : x->getAdj())
		{
			if (!v1 && KMPMatcher(street1, y.name))
			{
				e1 = y;
				v1 = x;

				if (v2)
					break;
			}
			else if (!v2 && KMPMatcher(street2, y.name))
			{
				e2 = y;
				v2 = x;

				if (v1)
					break;
			}
		}

		if (v1 && v2)
			break;
	}

	if (v1 && v2)
	{
		//verificar se as edges se cruzam e se têm sharing location no cruzamento

		if (e1.dest == e2.dest)
		{
			if (string(typeid(*e1.dest->getInfo()).name()) == "class SharingLocation")
				location = e1.dest;
		}
		else if (e1.dest == v2)
		{
			if (string(typeid(*v2->getInfo()).name()) == "class SharingLocation")
				location = v2;
		}
		else if (e2.dest == v1)
		{
			if (string(typeid(*v1->getInfo()).name()) == "class SharingLocation")
				location = v1;
		}

		return true;
	}
	return false;
}

vector<string> Graph::findSLApproximate(string street1, string street2)
{
	return vector<string>();
}
