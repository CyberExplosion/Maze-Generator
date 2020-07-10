#include "DjikstraPath.h"
using namespace std;

//The function will connect 2 nodes with an two way edge
void ListGraph::connectVertices(const Node& node1, const Node& node2) {

	auto result = find_if(begin(graph), end(graph),
		[&node1](list<Node>& host) {
			return (host.front().position.x == node1.position.x) && (host.front().position.y == node1.position.y);	//Find if the node1 already exist in graph
		});
	auto result2 = find_if(begin(graph), end(graph),
		[&node2](list<Node>& host) {
			return (host.front().position.x == node2.position.x) && (host.front().position.y == node2.position.y);	//Find if the node2 already exist in graph
		});
	if (result != end(graph))
		throw NodeNotExist("Node 1 does not exist");
	if (result2 != end(graph))
		throw NodeNotExist("Node 2 does not exist");
	//Edge 1 -> 2

	if (result == end(graph)) {
		result->push_back(node2);
	}

	//Edge 2->1
	if (result2 == end(graph)) {
		result2->push_back(node1);
	}
}

//The function will put weight onto the nodes surround the input Node
void ListGraph::putWeight(const Node& inNode) {
	auto host = find_if(begin(graph), end(graph),
		[&inNode](list<Node>& host) {
			return (host.front().position.x == inNode.position.x) && (host.front().position.y == inNode.position.y);	//Find the node that want to put weight to its' neighbor
		});

	if (host != end(graph)) {	//If the node is in a graph, it will always have at least an edge -> always have at least one neighbor
		for (auto neighbor = ++begin(*host); neighbor != end(*host); ++neighbor) {	// ++ will make the iterator move to the 2nd node and not the first node which is host
			neighbor->weightToHere = host->front().weightToHere + 1;
		}
	}
	else throw OnlyOneNodeExist("There's no neighbor node to put weight on");
}

//Function check if the two position is adjacent or not
bool ListGraph::isAdjacent(const Pos& node1, const Pos& node2) noexcept {
	if (node1.x == node2.x)
		if (node1.y == node2.y - 1 || node1.y == node2.y + 1)
			return true;
	if (node1.y == node2.y)
		if (node1.x == node2.x - 1 || node1.x == node2.x + 1)
			return true;
	return false;
}

//Function will connect the two nodes if possible
bool ListGraph::connect(const Node& node1, const Node& node2) {
	if (isAdjacent(node1.position, node2.position)) {
		try {
			connectVertices(node1, node2);
			return true;
		}
		catch (const ListGraph::NodeNotExist& e) {
			e.what();
			throw;
		}
	}
	return false;
}

//Considering the first node already been made with weight 0 when class was first init
void ListGraph::addNode(const Node& preMadeNode) noexcept {
	if (graph.size() == 1) {
		Pos neoPos(preMadeNode.position.x, preMadeNode.position.y);
		Node neoNode(neoPos);
		list<Node> neoHost;
		neoHost.push_front(std::move(neoNode));	//Ain't gona use the neoNode afterward anyway, just move into the host

		graph.push_back(neoHost);
	}
	else {
		//FIXME: Add functionality to deal with when the graph already has more than 1 node
	}
}
