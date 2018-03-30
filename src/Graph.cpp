#include "Graph.h"

Vertex::Vertex(Location in) : info(in) {}

void Vertex::addEdge(Vertex *d, double w, unsigned long long id, string name) {
	adj.push_back(Edge(d, w, id, name));
}

bool Vertex::operator<(Vertex & vertex) const {
	return this->dist < vertex.dist;
}


Location Vertex::getInfo() const {
	return this->info;
}


double Vertex::getDist() const {
	return this->dist;
}


Vertex *Vertex::getPath() const {
	return this->path;
}


Edge::Edge(unsigned long long id, Vertex *d, double w) : id(id), dest(d), weight(w) {}

inline Edge::Edge(Vertex* d, double w, unsigned long long id, string name) : id(id), dest(d), weight(w), name(name) {}

inline Edge::Edge(unsigned long long id, Vertex* d) : id(id), dest(d) {}

int Graph::getNumVertex() const {
	return vertexSet.size();
}


vector<Vertex *> Graph::getVertexSet() const {
	return vertexSet;
}

Vertex * Graph::findVertex(const Location &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

bool Graph::addVertex(const Location &in) {
	if (findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex(in));
	return true;
}

bool Graph::addEdge(const Location* sourc, const Location* dest, double w, unsigned long long id, string name) {
	auto v1 = findVertex(*sourc);
	auto v2 = findVertex(*dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2, w, id, name);
	return true;
}

/**************** Single Source Shortest Path algorithms ************/


Location Graph::dijkstraShortestPath(const Location &origin) {
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

	Vertex *min=nullptr;

	while (!queue.empty() && (min == nullptr || !min->info.isAvailable()))
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
				if (oldDist == DBL_MAX)
					queue.insert(v2);
				else
					queue.decreaseKey(v2);
			}
		}
	}

	return min->getInfo();
}

vector<Vertex> Graph::getPath(const Location &origin, const Location &dest) const {
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