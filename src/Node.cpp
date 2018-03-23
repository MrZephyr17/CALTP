#include "Node.h"

using namespace std;

Node::Node()	// default
{
	this->id = 0;
	this->name = "DEFAULT";
	this->coords.x = 0;
	this->coords.y = 0;
}


Node::~Node() {}

Node::Node(int id, std::string name, int x, int y)
{
	this->id = id;
	this->name = name;
	this->coords.x = x;
	this->coords.y = y;
}

int Node::getID() const
{
	return this->id;
}

string Node::getName() const
{
	return this->name;
}

int Node::getXcoord() const
{
	return this->coords.x;
}

int Node::getYcoord() const
{
	return this->coords.y;
}

Point Node::getCoords() const
{
	return this->coords;
}

bool Node::operator==(const Node &n2) const
{
	return
		this->coords.x == n2.coords.x
		&&
		this->coords.y == n2.coords.y;
}

void Node::setID(int newID)
{
	this->id = newID;
}