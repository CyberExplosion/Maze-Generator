#include "DjikstraPath.h"
#include <string>
using namespace std;

//The function will connect 2 nodes with an two way edge
void ListGraph::connectVertices(const Node& node1, const Node& node2) {

	auto result = graph.count(node1);
	auto result2 = graph.count(node2);

	if (result == 0)
		throw NodeNotExist("Node 1 does not exist");
	if (result2 == 0)
		throw NodeNotExist("Node 2 does not exist");

	//Edge 1 -> 2
	graph[node1].push_back(node2);
	//Edge 2 -> 1
	graph[node2].push_back(node1);
	//result2->push_back(node1);
}


//The function will put weight onto the nodes surround the input Node
//void ListGraph::putWeight(const Node& inNode) {
//	auto host = find_if(begin(graph), end(graph),
//		[&inNode](list<Node>& host) {
//			return (host.front().position.x == inNode.position.x) && (host.front().position.y == inNode.position.y);	//Find the node that want to put weight to its' neighbor
//		});
//
//	if (host != end(graph)) {	//If the node is in a graph, it will always have at least an edge -> always have at least one neighbor
//		for (auto neighbor = ++begin(*host); neighbor != end(*host); ++neighbor) {	// ++ will make the iterator move to the 2nd node and not the first node which is host
//			neighbor->weightToHere = host->front().weightToHere + 1;
//		}
//	}
//	else throw OnlyOneNodeExist("There's no neighbor node to put weight on");
//}

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
			std::cerr << e.what();
			throw;
		}
	}
	return false;
}

void ListGraph::addAloneNode(const Node& awayNode) noexcept {
	list<Node> neoHost;
	neoHost.push_back(awayNode);
	graph[awayNode] = std::move(neoHost);	//Create new vertix for inNode
}

//Function will check for each adjacent direction of the inputNode (left, right, up, down)
//Add inNode to the graph if it's not already exists. Also connect the inNode edges with 
//each adjacent nodes if found
void ListGraph::addNodeToAlreadyExist(const Node& inNode) {
	if (graph.count(inNode) == 0) {
		bool addSuccess = false;
		//Left
		try {
			Pos left(inNode.position.x - 1, inNode.position.y);
			Node temp1(left);
			//Check for if node exist inside graph
			if (graph.count(temp1) > 0) {
				addAloneNode(inNode);
				addSuccess = connect(temp1, inNode);	//Connect edges of these 2 vertices
			}
		}
		catch (const Pos::NegativePos& e) { cerr << e.what(); }
		//Right
		Pos right(inNode.position.x + 1, inNode.position.y);
		Node temp2(right);
		//Check for if node exist inside graph
		if (graph.count(temp2) > 0) {
			if (graph.count(inNode) == 0) {	//If node is not already add on by the previous adjacent check
				addAloneNode(inNode);
			}
			addSuccess = connect(temp2, inNode);	//Connect edges of these 2 vertices
		}
		//Up
		try {
			Pos up(inNode.position.x, inNode.position.y - 1);
			Node temp3(up);
			//Check for if node exist inside graph
			if (graph.count(temp3) > 0) {
				if (graph.count(inNode) == 0)
					addAloneNode(inNode);
				addSuccess = connect(temp3, inNode);	//Connect edges of these 2 vertices
			}
		}
		catch (const Pos::NegativePos& e) { cerr << e.what(); }
		//Down
		Pos down(inNode.position.x, inNode.position.y + 1);
		Node temp4(down);
		//Check for if node exist inside graph
		if (graph.count(temp4) > 0) {
			if (graph.count(inNode) == 0)	//These checks could be made a lot better by doing it only one at the top. FIXME: Optimization opportunity
				addAloneNode(inNode);
			addSuccess = connect(temp4, inNode);	//Connect edges of these 2 vertices
		}
		if (!addSuccess)
			throw NodeNotAdjToAny("Node trying to add is not adjacent to any in graph");
	}
	else throw NodeAlreadyExist("Node trying to check before adding into graph already exist in graph");
}


////Function will check the 4 adjacent node surround this node and to see if any of them is in the graph
//bool ListGraph::existAround(const Node& node, Node& outNode) noexcept {
//	if (graph.count(node) == 0) {	//First check if this node is not in our graph
//		//Left
//		try {
//			Pos left(node.position.x - 1, node.position.y);
//
//			Node temp1(left);
//			auto resultL = graph.find(temp1);
//			if (resultL != end(graph)) {
//				outNode = resultL->first;
//				return true;
//			}
//		}
//		catch (const Pos::NegativePos& e) {}
//		//Right
//		Pos right(node.position.x + 1, node.position.y);
//		Node temp2(right);
//		auto resultR = graph.find(temp2);
//		if (resultR != end(graph)) {
//			outNode = resultR->first;
//			return true;
//		}
//		//Up
//		try {
//			Pos up(node.position.x, node.position.y - 1);
//			Node temp3(up);
//			auto resultU = graph.find(temp3);
//			if (resultU != end(graph)) {
//				outNode = resultU->first;
//				return true;
//			}
//		}
//		catch (const Pos::NegativePos& e) {}
//		//Down
//		Pos down(node.position.x, node.position.y + 1);
//		Node temp4(down);
//		auto resultD = graph.find(temp4);
//		if (resultD != end(graph)) {
//			outNode = resultD->first;
//			return true;
//		}
//	}
//	return false;
//}

//bool ListGraph::isSameNode(const Node& node1, const Node& node2) noexcept {
//	if (node1.position == node2.position)
//		return true;
//	return false;
//}

//Considering the first node already been made with weight 0 when class was first init
//WARNING: only use after knowing that the maze has been finalized and no more node is locked or unlocked
void ListGraph::addNode(Node& preMadeNode) {
	if (graph.empty()) {
		Pos neoPos(preMadeNode.position.x, preMadeNode.position.y);
		Node neoNode(neoPos);
		list<Node> neoHost;
		neoHost.push_front(neoNode);

		graph[neoNode] = std::move(neoHost);	//Neonode is the key for the Host
	}
	else {
		//auto host = graph.back();	//The new node to add is either adjacent to the preMadeNode or not
		if (graph.count(preMadeNode) > 0) {
			string msg = "New node already exist in the graph. Node position x: ";
			msg = msg + to_string(preMadeNode.position.x);
			msg += " y: ";
			msg += to_string(preMadeNode.position.y);
			throw NodeAlreadyExist(msg.c_str());
		}

		addNodeToAlreadyExist(preMadeNode);
		//weightUpdate(preMadeNode);
		//Pos temp(0, 0);
		//Node tmp(temp);

		//FIXME: Currently function existAround doesn't work like intended. It doesn't add in the 4th adjacent node in the main(). Also it only add 1 adjacent node and not all adjacent node to its neighbor list
		//if (existAround(preMadeNode, tmp)) {
			//if (preMadeNode.weightToHere == 0)	//Only update weight if the node was freshly made
				//preMadeNode.weightToHere = tmp.weightToHere + 1;	//Add weight to the adjacent node

			////add the new node
			//list<Node> neoHost;
			//neoHost.push_back(preMadeNode);
			//graph[preMadeNode] = std::move(neoHost);

			//connect(preMadeNode, tmp);	//Connect the edges between the last node and the new one
		//}
		//else throw logic_error("Input node is not adjacent to latest node in the graph");
	}
}

//This function return true if 2 nodes are connected in the graph, return false otherwise. Could throw
bool ListGraph::isConnected(const Node& firstNode, const Node& secondNode) {
	if (graph.count(firstNode) == 0)
		throw NodeNotExist("First param Node does not exist");
	if (graph.count(secondNode) == 0)
		throw NodeNotExist("Second param Node does not exist");

	auto host = graph[firstNode];
	for (auto neighbor : host)
		if (neighbor == secondNode)
			return true;

	return false;
}


//function returns a vector of Node that adjacent to the source, could throw if the source does not exist
std::vector<Node> ListGraph::nodesAdjcTo(const Node& source) {
	if (graph.count(source) == 0)	//FIXME: doesn't work if other source change the weight of a particular node.
									//Want to just check the position of a node and not it's weight also. ONLY WAY is to implement weight on a different containers
		throw NodeNotExist("Source node does not exist in graph");
	vector<Node>temp;
	list<Node> host = graph[source];
	for (auto neighbor : host) {
		temp.push_back(neighbor);
	}
	return temp;
}


////This function will add weight into the newly add nodes base on the weight of existing nodes
//void ListGraph::weightUpdate(const Node& inNode)  {
//	/*
//	Function will find the lowest weight in the list, then base on that make the host only 1 more weight away
//	This deals with situation when a new node is created adjacent between a low weight node and a high weight node
//	*/
//	auto host = graph.find(inNode);
//	if (host != end(graph)) {
//		int lowestWeight = host->second.front().weightToHere;
//		for (const auto& neighbors : host->second) {
//			if (neighbors.weightToHere < lowestWeight)
//				lowestWeight = neighbors.weightToHere;
//		}
//		for (auto& neighbor : host->second) {
//			neighbor.weightToHere = lowestWeight + 1;
//		}
//
//	}
//	else throw NodeNotExist("Node to update weight is not existed in graph");
//}

//The function will clean out the node inside the stack that doesn't corresponding to a prevNode
void DjikstraPath::pruneAndReversePath(stack<DjisktraNode>& tempPath) noexcept {
	DjisktraNode curNode = tempPath.top();	//Get the end Node
	tempPath.pop();

	while (!tempPath.empty()) {
		if (tempPath.top().position == curNode.prevPos) {	//When the prevPos is found in the tempPath
			curNode = tempPath.top();
			pathWay.push(curNode);
		}
		tempPath.pop();
	}
}

//The weight is updated after graph being created.
void DjikstraPath::generateWeight(ListGraph& t_graph) {

}

void DjikstraPath::pathFinding() {
	stack<DjisktraNode> tempPath;
	set<DjisktraNode, lowerCompSet> prioritySet;
	unordered_set<DjisktraNode, DjikstraSetHash> visited;
	unordered_map<Pos, int, DjikstraMapHash> weightMap;	//FIXME: Doesn't work like thinking. If stays like the model, the prev Path and the position combine will be the key. THe weight is the value
	//unordered_set<int> visited;

	//Set the begin node as the current Node
	prioritySet.insert(startNode);
	weightMap[startNode.position] = 0;	//start node always has weight 0

	while (true) {
		DjisktraNode curNode = std::move(*prioritySet.begin());	//The front of set is going to be the lowest weight
		prioritySet.erase(prioritySet.begin());	//Erase the empty spot
		visited.insert(curNode);
		tempPath.push(curNode);

		//Push all the adj nodes to currNode into set
		vector<Node> neighbors = std::move(graph.nodesAdjcTo(curNode));	//Hope this will call the master class version

		for (Node& each : neighbors) {
			DjisktraNode neoNode(each, false, curNode.position);
			if (neoNode.position == endNode.position) {	//If end node is one of the neighbors
				tempPath.push(neoNode);
				pruneAndReversePath(tempPath);
				return;		//Once find the endNode, it's finished
			}
			if (visited.count(neoNode) == 0) {	//Only add into the adjacent set if the node is NOT VISITED
												//Push all the adjacent node (with prevPos) into the set. Only unvisited Node tho
				//Use new Map to fix the problem. FIXME: The map needs to be a pair<position, previous path> as key and weight as value if wants to follow EXACTLY like the model
				if (curNode != each) {
					weightMap[each.position] = weightMap[curNode.position] + 1;	//Distance to each adjacent node is 1 away from the current Node
				}

				auto duplicatedNode = prioritySet.find(each);
				if (duplicatedNode != end(prioritySet)) {	//Duplication happen
					if (weightMap[neoNode.position] < weightMap[duplicatedNode->position]) {	//If the weight is lower than one already exist in set
						//Then update the node in set to be the new weight and change its' prevNode path
						prioritySet.erase(duplicatedNode);	//Then delete the old Node
					}
				}
				prioritySet.insert(neoNode);	//First insert the better new node
			}
		}
	}
}
