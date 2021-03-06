#include <iostream>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <queue>
#include <stack>
//#include <array>
//#include <future>
#include <string>
#include "DjikstraPath.h"
using namespace std;

//0 - 0 pos will be the top left corner, the y pos will be Cartesian Coordinate but downward
//struct Pos {
//	int x;
//	int y;
//	Pos(int localX, int localY) : x(localX), y(localY) {};
//};



//Prototype: The maze won't have a dead end path
class Maze {
private:
	//int width;
	//int length;
	char obstacleChar;
	char pathChar;
	Pos startNode;
	Pos endNode;
	vector<vector<char>> m_maze;

	//Graph
	ListGraph graph;
	//DjikstraPath graph;

	bool adjToEndNode(const Pos& local);
	bool validPos(const Pos& local);
	bool unlockNode(const Pos& local);
	Pos findAdjNodeToUnlock(const Pos& curLocal);
	void generatingMaze();
	std::stack<DjisktraNode> generatedPathWay() const;
public:

	Maze(int width, int length, Pos start, Pos end, char obst = 'X', char path = '.') : obstacleChar(obst), pathChar(path), startNode(start), endNode(end), graph(start) {
		m_maze = vector<vector<char>>(width, vector<char>(length, obst));
		//Node s(start);
		//graph = ListGraph(s);
		generatingMaze();
	}
	void getMaze(vector<vector<char>>& source) {
		source = m_maze;
	}

	void mazeWithSolPath(std::vector<std::vector<char>>& mazeWithSol, const char pathChar);
	//Error class
	struct no_sides_avaialbe : public logic_error {
		no_sides_avaialbe(const char* msg) : logic_error(msg) {};
	};
};

int main() {
	Maze maze(10, 10, Pos(0,0), Pos(5,5));
	vector<vector<char>> result;
	maze.getMaze(result);

	for (auto row : result) {
		for (auto col : row) {
			cout << col << " ";
		}
		cout << "\n";
	}
	//auto path = maze.generatedPathWay();
	maze.getMaze(result);	//Maze correctly produces and the graph report accurately. Graph doesn't include the destination nodes but every other nodes is in there
	cout << "End\n Result:\n";
	maze.mazeWithSolPath(result, '0');
	for (auto row : result) {
		for (auto col : row) {
			cout << col << " ";
		}
		cout << "\n";
	}
	return 0;
}

bool Maze::adjToEndNode(const Pos& local) {
	if (validPos(local)) {
		if (local.x == endNode.x)
			if (local.y == endNode.y - 1 || local.y == endNode.y + 1)
				return true;
		if (local.y == endNode.y)
			if (local.x == endNode.x - 1 || local.x == endNode.x + 1)
				return true;
	}
	return false;
}

bool Maze::validPos(const Pos& local) {
	if ((local.x >= m_maze.at(0).size() || local.y >= m_maze.size()) || (local.x < 0 || local.y < 0)) {
		return false;
	}
	return true;
}

//Function return true if the node can be unlocked, return false if already unlocked or out of bound
bool Maze::unlockNode(const Pos& local) {
	if (validPos(local)) {
		if (m_maze.at(local.y).at(local.x) == obstacleChar) {
			m_maze.at(local.y).at(local.x) = pathChar;
			return true;
		}
	}
	return false;
}

//The function find the next "Locked" node, unlock it and return the location of that node, if not found the function throw a logical error
Pos Maze::findAdjNodeToUnlock(const Pos& curLocal) {
	Pos targetLocal = curLocal;

	if (validPos(curLocal)) {
		//Direction dirNodeToUnlock = static_cast<Direction>(rand() / (RAND_MAX / (3 - 0 + 1) + 1));
		vector<Direction> DirVec{ Direction::left, Direction::right, Direction::up, Direction::down };

		//srand(time(0));
		auto r = rand() / (RAND_MAX / (DirVec.size() - 0) + 1);	//Get a random index from 0 to last index in DirVec
		//auto r = rand() % DirVec.size();

		switch (DirVec.at(r)) {
		case Direction::left:
			targetLocal.x -= 1;
			break;
		case Direction::right:
			targetLocal.x += 1;
			break;
		case Direction::up:
			targetLocal.y -= 1;
			break;
		case Direction::down:
			targetLocal.y += 1;
			break;
		case Direction::invalid:
		default:
			break;
		}

		if (!unlockNode(targetLocal)) {	//Random direction doesn't work, will go from beginning to end of vector
			for (auto dir : DirVec) {
				targetLocal = curLocal;	//Reset the location
				switch (dir) {
				case Direction::left:
					targetLocal.x -= 1;
					break;
				case Direction::right:
					targetLocal.x += 1;
					break;
				case Direction::up:
					targetLocal.y -= 1;
					break;
				case Direction::down:
					targetLocal.y += 1;
					break;
				case Direction::invalid:
				default:
					break;
				}
				if (unlockNode(targetLocal))
					return targetLocal;
			}
		}
		else return targetLocal;

		throw no_sides_avaialbe("Reach end of 4 direction without finding any possible Node to unlock");
	}
	else throw invalid_argument("Position of node is not valid. x:" + to_string(curLocal.x) + ", y: " + to_string(curLocal.y));
	//return targetLocal;
}

/*******************
The function create the maze by using random direction, and use a technique like 
Breadth first search to put create path from the start Node to the end Node
********************/
void Maze::generatingMaze() {
	//Check if the starting and end position is valid
	if (validPos(startNode) && validPos(endNode)) {
		if (adjToEndNode(startNode))
			return;
		//Fill the start Node with an opening path
		m_maze.at(startNode.y).at(startNode.y) = pathChar;

		//Randomly determine how many Path should be created
		//M + rand() / (RAND_MAX / (N - M + 1) + 1)
		srand(time(0));
		queue<Pos> Nodes;
		stack<Pos> prevNodes;
		Nodes.push(startNode);

		while (!Nodes.empty()) {
			//int numUnlock = 1 + rand() / (RAND_MAX / (3 - 1 + 1) + 1);	//3 is the maximum number of adjacent nodes that you can unlock
			//int numUnlock = 1 + rand() % 2;
			int numUnlock = 1;	// FIXME: Only unlock 1 adjacent nodes at a time, any higher cause problem for the whole maze to be unlocked

			Pos curNode = Nodes.front();
			Nodes.pop();

			//Add current node to the graph - GRAPH
			Node neoNode(curNode);
			try {
				graph.addNode(neoNode);
				if (neoNode == endNode) {	//Reach the end
					return;
				}
			}
			catch (const ListGraph::NodeAlreadyExist& e) {
				cerr << e.what() << "\n";
			}
			//FIXME: The maze not correctly generated - DONE
			for (auto row : m_maze) {
				for (auto col : row) {
					cout << col << " ";
				}
				cout << endl;
			}
			cout << "/////////////////////////////\n";
			////////////////////////////////////////////////////////

			//FIXME: There's a possibility of the maze path stuck at the left wall of the box because the first Direction fill in findAdjNodeToUnlock is left first - DONE
			//if (!adjToEndNode(curNode)) {
				for (int i = 0; i < numUnlock; ++i) {
					try {
						Pos adj = findAdjNodeToUnlock(curNode);	//Could throw right here if there's no side available
						Nodes.push(adj);

						//Apply the stack tracking method with nodes that still has adjacent sides
						prevNodes.push(Nodes.back());
					}
					catch (const no_sides_avaialbe&) {	//No sides is available -> Use nodes that has available sides store in the stack
						if (!prevNodes.empty()) {
							Nodes.push(prevNodes.top());
							prevNodes.pop();
						}
						continue;	//If no locked node is found, then just skip over 1 loop
					}
				}
			//}
			//else return;	//Finish creating a path toward the end Nodes
		}
	}
}

//Get path from the Dijkstra
std::stack<DjisktraNode> Maze::generatedPathWay() const {
	//DjisktraNode start(startNode);
	//DjisktraNode end(endNode);
	DjikstraPath pathWay(graph, startNode, endNode);

	return pathWay.getPath();
}

//Copy out maze with the pathway using the pathChar
void Maze::mazeWithSolPath(std::vector<std::vector<char>>& mazeWithSol, const char pathChar) {
	auto path = generatedPathWay();
	mazeWithSol = m_maze;
	while (!path.empty()) {
		mazeWithSol.at(path.top().position.y).at(path.top().position.x) = pathChar;
		path.pop();
	}
}
