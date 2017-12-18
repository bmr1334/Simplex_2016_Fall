#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
using namespace std;

struct DIRECTION
{
	string nodeName; //name of the node
	bool exists; //true if the node being checked for exists
};

//properties of the nodes
typedef struct _NODE
{
	//directional flags for node connections
	DIRECTION up;
	DIRECTION down;
	DIRECTION left;
	DIRECTION right;

	string thisName; //the name of the current node

	int cost; //cost of traveling from this node to the goal
}NODE;

class AStar
{
public:
	bool upOrRight; //true if up path is shorter, false if right path is shorter
	std::vector<NODE> openSet; //unevaluated discovered nodes
	std::vector<string> closedSet; //already evaluated nodes
	std::vector<NODE> allNodes; //all nodes in the map
	NODE travelTo; //node being looked at by search
	int distTravelled; //number of moves made during search

	//use A* search algorithm to find best path
	//returns true if going up is shorter
	//returns false if going right is shorter
	bool RunSearch();

	//constructor
	AStar();

private:
	//variables
	NODE block1; //nodes representing the grid that the algorithm searches through
	NODE block2;
	NODE block3;
	NODE block4;
	NODE block5;
	NODE block6;
	NODE block7;
	NODE block8;
	NODE block9;
	NODE block10;
	NODE block11;
	NODE block12;
	NODE block13;
	NODE block14;
	NODE block15;
	NODE block16;
	NODE block17;
	NODE block18;

	//cost of getting from starting point to where we are right now
	int currentCost;
	
	//override == operator
	friend bool AStar::operator==(NODE &n1, const NODE &n2);
};