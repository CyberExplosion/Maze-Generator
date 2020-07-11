#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <queue>

const int IFN = 9999;

template<typename T>
struct inequal {
	bool operator!=(const T& rhs) const {
		return !(static_cast<const T&>(*this) == rhs);
	}
};

struct Pos : inequal<Pos> {
	//Error class
	struct NegativePos : public std::runtime_error {
		NegativePos(const char* msg) : runtime_error(msg) {};
	};
	int x;
	int y;
	Pos(const int locaX, const int locaY) : x(locaX), y(locaY) {
		if (x < 0 || y < 0)
			throw NegativePos("Negative coordinates are not allowed");
	};
	bool operator==(const Pos& rhs) const {
		return (x == rhs.x && y == rhs.y);
	}
};

struct Node {
	Pos position;
	int weightToHere;
	Node(const Pos& pos, const int w = 0) noexcept : position(pos), weightToHere(w){};
	bool operator==(const Node& rhs) const {
		return this->position == rhs.position;
	}
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
	struct NodeAlreadyExist : public std::logic_error {
		NodeAlreadyExist(const char* msg) : logic_error(msg) {};
	};
private:
	std::vector<std::list<Node>> graph;
	//Private func
	void connectVertices(const Node& node1, const Node& node2);
	//void putWeight(const Node& inNode);	//Not used
	//FIXME: create method to determine WHEN to connect edges between 2 nodes - DONE
	//FIXME: create method to find if there's exist in the graph nodes that adjacent to one particular Input node
	bool isAdjacent(const Pos& node1, const Pos& node2) noexcept;
	bool connect(const Node& node1, const Node& node2);
	//bool isSameNode(const Node& node1, const Node& node2) noexcept;
public:
	ListGraph(Node& preNode) {
		//preNode.weightToHere = 0;
		try {
			addNode(preNode);
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << "\n";
		}
	}
	//Check if the node exist, if not then add it into the graph.
	void addNode(Node& preMadeNode);
};

class DjikstraPath {
	
};

