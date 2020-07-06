#include <iostream>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <queue>
#include <array>
using namespace std;

//0 - 0 pos will be the top left corner, the y pos will be Cartesian Coordinate but downward
struct Pos {
	int x;
	int y;
	Pos(int localX, int localY) : x(localX), y(localY) {};
};

enum class Direction { left, right, up, down, invalid };

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

	bool adjToEndNode(Pos local);
	bool validPos(Pos local);
	bool unlockNode(Pos local);
	Pos findAdjNodeToUnlock(Pos curLocal);
	void generatingMaze();
public:
	Maze(int width, int length, Pos start, Pos end, char obst = 'X', char path = 'O') : obstacleChar(obst), pathChar(path), startNode(start), endNode(end) {
		m_maze = vector<vector<char>>(width, vector<char>(length, obst));
	}
	void getMaze(vector<vector<char>>&& source) {
		source = std::move(m_maze);
	}
};

int main() {
	Pos first(0, 0);
	Pos end(2, 2);
	Maze maze(3, 3, first, end);
	cout << "Paused";
}

bool Maze::adjToEndNode(Pos local) {
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

bool Maze::validPos(Pos local) {
	if ((local.x >= m_maze.at(0).size() || local.y >= m_maze.size()) || (local.x < 0 || local.y < 0)) {
		return false;
	}
	return true;
}

//Function return true if the node can be unlocked, return false if already unlocked or out of bound
bool Maze::unlockNode(Pos local) {
	if (validPos(local)) {
		if (m_maze.at(local.y).at(local.x) == pathChar)
			return false;
		else {
			m_maze.at(local.y).at(local.x) = pathChar;
			return false;
		}
	}
	return false;
}
//The function find the next "Locked" node and return the location of that node, if not found the function throw a logical error
Pos Maze::findAdjNodeToUnlock(Pos curLocal) {
	Pos targetLocal = curLocal;

	if (validPos(curLocal)) {
		//Direction dirNodeToUnlock = static_cast<Direction>(rand() / (RAND_MAX / (3 - 0 + 1) + 1));
		bool foundNodeToUnlock = false;
		vector<Direction> DirVec{ Direction::left, Direction::right, Direction::up, Direction::down };
		srand(time(0));
		auto r = rand() / (RAND_MAX / (DirVec.size() - 0) + 1);	//Get a random index from 0 to last index in DirVec

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
	}
	throw logic_error("Reach end of 4 direction without finding any possible Node to unlock");
	//return targetLocal;
}

void Maze::generatingMaze() {
	//Check if the starting and end position is valid
	if (validPos(startNode) && validPos(endNode)) {
		if (adjToEndNode(startNode))
			return;
		//Randomly determine how many Path should be created
		//M + rand() / (RAND_MAX / (N - M + 1) + 1)
		srand(time(0));
		queue<Pos> Nodes;
		Nodes.push(startNode);

		while (!Nodes.empty()) {
			int numUnlock = 1 + rand() / (RAND_MAX / (3 - 1 + 1) + 1);	//3 is the maximum number of adjacent nodes that you can unlock
			Pos curNode = Nodes.front();
			Nodes.pop();
			if (!adjToEndNode(curNode)) {
				for (int i = 0; i < numUnlock; ++i) {
					try {
						Pos adj = findAdjNodeToUnlock(curNode);
						Nodes.push(adj);
					}
					catch (const logic_error& e) {
						continue;	//If no node is found, then just skip over 1 loop
					}
				}
			}
			else return;
		}
	}
}
