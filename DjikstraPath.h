#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <queue>

const int IFN = 9999;

struct Pos {
	int x;
	int y;
	Pos(const int locaX, const int locaY) : x(locaX), y(locaY) {};
};

struct Node {
	Pos position;
	int weightToHere;
	Node(const Pos& pos, const int w = IFN) : position(pos), weightToHere(w){};
};

class ListGraph {
public:
	//Error classes
	struct NodeNotExist : public std::logic_error {
		NodeNotExist(const char* msg) : logic_error(msg) {};
	};
	struct OnlyOneNodeExist : public std::logic_error {
		OnlyOneNodeExist(const char* msg) : logic_error(msg) {};
	};
private:
	std::vector<std::list<Node>> graph;
	//Private func
	void connectVertices(const Node& node1, const Node& node2);
	void putWeight(const Node& inNode);
	//FIXME: create method to determine WHEN to connect edges between 2 nodes
	//FIXME: create method to find if there's exist in the graph nodes that adjacent to one particular Input node
	bool isAdjacent(const Pos& node1, const Pos& node2) noexcept;
	bool connect(const Node& node1, const Node& node2);
public:
	//Check if the node exist, if not then add it into the graph.
	void addNode(const Node& preMadeNode);
};

class DjikstraPath {
	
};

