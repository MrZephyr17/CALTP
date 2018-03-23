// Node.h
#pragma once

#ifndef NODE_H
#define NODE_H

#include <vector>

class Graph;

typedef struct
{
	int x;
	int y;
}Point;

/************************* NODE *************************/

/*
		ACRESCENTAR O QUE É PRECISO AO NODE,
		
		MENOS O QUE O GRAPH TEMPLATE Vertex JÁ POSSUI
*/

class Node
{
	int id;				///< Node's ID
	std::string name;	///< Node's name
	Point coords;		///< Node's coordinates (x,y)

public:
	Node();

	~Node();

	Node(int id, std::string name, int x, int y);

	int getID() const;

	std::string getName() const;

	int getXcoord() const;

	int getYcoord() const;

	Point getCoords() const;

	bool operator==(const Node &n2) const;

	void setID(int newID);

};

#endif /* NODE_H */