#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <set>

const int IFN = 9999;

enum class Direction { left, right, up, down, invalid };

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
	bool operator==(const Pos& rhs) const noexcept {
		return (x == rhs.x && y == rhs.y);
	}
};

struct Node : inequal<Pos>{
	Pos position;
	int weightToHere;
	Node(const Pos& pos, const int w = 0) noexcept : position(pos), weightToHere(w){};
	bool operator==(const Node& rhs) const noexcept {
		return this->position == rhs.position;
	}
};

//Using the hash combine method provides by cppreference
struct listHash {
	std::size_t operator()(const Node& host) const noexcept {
		std::size_t h1 = std::hash<int>{}(host.position.x);
		std::size_t h2 = std::hash<int>{}(host.position.y);
		std::size_t h3 = std::hash<int>{}(host.weightToHere);
		
		// Compute individual hash values for first,
	  // second and third and combine them using XOR
	  // and bit shifting:

		//return ((hash<string>()(k.first) ^ (hash<string>()(k.second) << 1)) >> 1) ^ (hash<int>()(k.third) << 1);

		return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
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
	struct NodeNotAdjToAny : public std::runtime_error {
		NodeNotAdjToAny(const char* msg) : runtime_error(msg) {};
	};
private:
	//std::vector<std::list<Node>> graph;	-- Using set provides faster access
	std::unordered_map<Node, std::list<Node>, listHash> graph;

	//Private func
	void connectVertices(const Node& node1, const Node& node2);
	//void putWeight(const Node& inNode);	//Not used
	//FIXME: create method to determine WHEN to connect edges between 2 nodes - DONE
	//FIXME: create method to find if there's exist in the graph nodes that adjacent to one particular Input node
	bool isAdjacent(const Pos& node1, const Pos& node2) noexcept;
	bool connect(const Node& node1, const Node& node2);
	//void checkAdjByDir(const Direction& dir);
	//bool existAround(const Node& inNode, Node& outNode) noexcept;
	//bool isSameNode(const Node& node1, const Node& node2) noexcept;
	void addAloneNode(const Node& awayNode) noexcept;
	void addNodeToAlreadyExist(const Node& inNode);
	//void weightUpdate(const Node& startNode);
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
	ListGraph(Pos& prePos) {	//Because of the way the parameter is set up, i had to ended up making another constructor. This could be make better using init list - OPTIMIZATION
		Node preNode(prePos);

		try {
			addNode(preNode);
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << "\n";
		}
	}
	//Check if the node exist, if not then add it into the graph.
	void addNode(Node& preMadeNode);
	bool isConnected(const Node& firstNode, const Node& secondNode);
	std::vector<Node> nodesAdjcTo(const Node& source);
};

struct DjisktraNode : public Node {
	bool visited;
	Pos prevPos;
	DjisktraNode(Pos curPos, int weight, bool visit = false, Pos prev = Pos(0, 0)) : Node(curPos, weight), visited(visit), prevPos(prev) {};
	DjisktraNode(Node curNode, Pos prev = Pos(0, 0)) : Node(curNode), prevPos(prev) {};
	bool operator==(const DjisktraNode& rhs) const noexcept{
		return (static_cast<Node> (*this) == static_cast<Node>(rhs));	//Use the Node version of equal
		//return this->position == rhs.position;
	}
};

//Node only hash equal the current node location. Since this is what the find() function use to find a duplicated node in the set. This is used to find lesser weight with different previous path
struct DjikstraSetHash {
	std::size_t operator()(const DjisktraNode& node) const noexcept {
		////return ((hash<string>()(k.first) ^ (hash<string>()(k.second) << 1)) >> 1) ^ (hash<int>()(k.third) << 1);
		////		std::size_t h1 = std::hash<int>{}(host.position.x);
		std::size_t h1 = std::hash<int>{}(node.position.x);
		std::size_t h2 = std::hash<int>{}(node.position.y);
		//std::size_t h3 = std::hash<int>{}(node.prevPos.x);
		//std::size_t h4 = std::hash<int>{}(node.prevPos.y);
		//std::size_t h5 = std::hash<bool>{}(node.visited);
		//std::size_t h6 = std::hash<int>{}(node.weightToHere);	//These codes above are utterly stupid

		//return (((((h1 ^ (h2 << 1) >> 1) ^ (h3 << 1)) >> 1) ^ (h4 << 1) >> 1) ^ (h5 << 1) >> 1 ) ^ (h6 << 1);
		return h1 ^ (h2 << 1);

		//return static_cast<std::size_t>(sizeof(node.position.x) + sizeof(node.position.y) + sizeof(node.prevPos.x) + sizeof(node.prevPos.y) + sizeof(node.visited) + sizeof(node.weightToHere));
	}
};

struct DjikstraMapHash {
	std::size_t operator()(const Pos& loca) const noexcept {
		std::size_t h1 = std::hash<int>{}(loca.x);
		std::size_t h2 = std::hash<int>{}(loca.y);
		return (h1 ^ (h2 << 1));
	}
};

class DjikstraPath {
private:
	ListGraph graph;
	Pos startNode, endNode;
	std::stack<DjisktraNode> pathWay;

	struct lowerCompSet {
		bool operator()(const DjisktraNode& lhs, const DjisktraNode& rhs) const {
			return lhs.weightToHere < rhs.weightToHere;
		}
	};

	void pruneAndReversePath(std::stack<DjisktraNode>& path) noexcept;
	void generateWeight(ListGraph& t_graph);
	void pathFinding();

public:
	//No output parameters in here
	DjikstraPath(const ListGraph& t_graph, const Pos& beginNode, const Pos& finishNode) : graph(t_graph), startNode(beginNode), endNode(finishNode) {
		pathFinding();
	};
	std::stack<DjisktraNode> getPath() {
		return pathWay;
	}
};

