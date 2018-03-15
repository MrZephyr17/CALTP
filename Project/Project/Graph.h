#pragma once
#include <vector>

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> > adj;  // list of outgoing edges
	bool visited;          // auxiliary field used by dfs and bfs
	int indegree;          // auxiliary field used by topsort
	bool processing;       // auxiliary field used by isDAG

	void addEdge(Vertex<T> *dest, double w);
	bool removeEdgeTo(Vertex<T> *d);
public:
	Vertex(T in);
	friend class Graph<T>;
};

template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(Vertex<T> *d, double w);
	public Vertex<T>* dest();
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set

	void dfsVisit(Vertex<T> *v, vector<T> & res) const;
	Vertex<T> *findVertex(const T &in) const;
	bool dfsIsDAG(Vertex<T> *v) const;
public:
	int getNumVertex() const;
	bool addVertex(const T &in);
	bool removeVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	bool removeEdge(const T &sourc, const T &dest);
	vector<T> dfs() const;
	vector<T> bfs(const T &source) const;
	vector<T> topsort() const;
	int maxNewChildren(const T &source, T &inf) const;
	bool isDAG() const;
};

template<class T>
inline void Vertex<T>::addEdge(Vertex<T>* dest, double w)
{	
	adj.push_back(Edge(dest, w));
}

template<class T>
inline bool Vertex<T>::removeEdgeTo(Vertex<T>* d)
{
	auto it = adj.find_if(adj.begin(), adj.end(), [&d](Edge<T> e) {
		return e.dest() == d;
	 });
	
	if (it == adj.end())
		return false;

	adj.erase(it);
	return true;
}

template<class T>
inline Vertex<T>::Vertex(T in)
{
	info = in;
}

template<class T>
inline Edge<T>::Edge(Vertex<T>* d, double w)
{
	if (d == nullptr)
		throw invalid_argument("No such vertex!");

	if (w < 0)
		throw invalid_argument("Weight cannot be negative!");

	dest = d;
	weight = w;
}

template<class T>
inline Vertex<T>* Edge<T>::dest()
{
	return dest;
}

template<class T>
inline void Graph<T>::dfsVisit(Vertex<T>* v, vector<T>& res) const
{
}

template<class T>
inline Vertex<T>* Graph<T>::findVertex(const T & in) const
{
	return NULL;
}

template<class T>
inline bool Graph<T>::dfsIsDAG(Vertex<T>* v) const
{
	return false;
}

template<class T>
inline int Graph<T>::getNumVertex() const
{
	return 0;
}

template<class T>
inline bool Graph<T>::addVertex(const T & in)
{
	return false;
}

template<class T>
inline bool Graph<T>::removeVertex(const T & in)
{
	return false;
}

template<class T>
inline bool Graph<T>::addEdge(const T & sourc, const T & dest, double w)
{
	return false;
}

template<class T>
inline bool Graph<T>::removeEdge(const T & sourc, const T & dest)
{
	return false;
}

template<class T>
inline vector<T> Graph<T>::dfs() const
{
	return vector<T>();
}

template<class T>
inline vector<T> Graph<T>::bfs(const T & source) const
{
	return vector<T>();
}

template<class T>
inline vector<T> Graph<T>::topsort() const
{
	return vector<T>();
}

template<class T>
inline int Graph<T>::maxNewChildren(const T & source, T & inf) const
{
	return 0;
}

template<class T>
inline bool Graph<T>::isDAG() const
{
	return false;
}
