#include "AStar.h"

//set up node connections and default values
AStar::AStar()
{
	//set up personal (non-linked) node names
	block1.thisName = "block1";
	block2.thisName = "block2";
	block3.thisName = "block3";
	block4.thisName = "block4";
	block5.thisName = "block5";
	block6.thisName = "block6";
	block7.thisName = "block7";
	block8.thisName = "block8";
	block9.thisName = "block9";
	block10.thisName = "block10";
	block11.thisName = "block11";
	block12.thisName = "block12";
	block13.thisName = "block13";
	block14.thisName = "block14";
	block15.thisName = "block15";
	block16.thisName = "block16";
	block17.thisName = "block17";
	block18.thisName = "goal"; //the end goal

	//directional links and names
	block1.up.exists = true;
	block1.up.nodeName = "block2";
	block1.right.exists = true;
	block1.right.nodeName = "block9";
	block2.up.exists = true;
	block2.up.nodeName = "block3";
	block2.down.exists = true;
	block2.down.nodeName = "block1";
	block3.up.exists = true;
	block3.up.nodeName = "block4";
	block3.down.exists = true;
	block3.down.nodeName = "block2";
	block4.up.exists = true;
	block4.up.nodeName = "block5";
	block4.down.exists = true;
	block4.down.nodeName = "block3";
	block5.down.exists = true;
	block5.down.nodeName = "block4";
	block5.right.exists = true;
	block5.right.nodeName = "block6";
	block6.left.exists = true;
	block6.left.nodeName = "block5";
	block6.right.exists = true;
	block6.right.nodeName = "block7";
	block7.down.exists = true;
	block7.down.nodeName = "block17";
	block7.left.exists = true;
	block7.left.nodeName = "block6";
	block7.right.exists = true;
	block7.right.nodeName = "block8";
	block8.left.exists = true;
	block8.left.nodeName = "block7";
	block8.right.exists = true;
	block8.right.nodeName = "goal"; //end goal is to the right
	block18.left.exists = true;
	block18.left.nodeName = "block8";
	block9.left.exists = true;
	block9.left.nodeName = "block1";
	block9.right.exists = true;
	block9.right.nodeName = "block10";
	block10.left.exists = true;
	block10.left.nodeName = "block9";
	block10.right.exists = true;
	block10.right.nodeName = "block11";
	block11.left.exists = true;
	block11.left.nodeName = "block10";
	block11.right.exists = true;
	block11.right.nodeName = "block12";
	block12.up.exists = true;
	block12.up.nodeName = "block13";
	block12.left.exists = true;
	block12.left.nodeName = "block11";
	block13.up.exists = true;
	block13.up.nodeName = "block14";
	block13.down.exists = true;
	block13.down.nodeName = "block12";
	block14.down.exists = true;
	block14.down.nodeName = "block13";
	block14.left.exists = true;
	block14.left.nodeName = "block15";
	block15.left.exists = true;
	block15.left.nodeName = "block16";
	block15.right.exists = true;
	block15.right.nodeName = "block14";
	block16.up.exists = true;
	block16.up.nodeName = "block17";
	block16.right.exists = true;
	block16.right.nodeName = "block15";
	block17.up.exists = true;
	block17.up.nodeName = "block7";
	block17.down.exists = true;
	block17.down.nodeName = "block16";

	//set up cost to travel from this node to the goal, lower is better
	block1.cost = 8;
	block2.cost = 7;
	block3.cost = 6;
	block4.cost = 5;
	block5.cost = 4;
	block6.cost = 3;
	block7.cost = 2;
	block8.cost = 1;
	block9.cost = 11;
	block10.cost = 10;
	block11.cost = 9;
	block12.cost = 8;
	block13.cost = 7;
	block14.cost = 6;
	block15.cost = 5;
	block16.cost = 4;
	block17.cost = 3;
	block18.cost = 0;

	//start openSet off at the start
	openSet.push_back(block1);

	//12 because that's the highest possible cost of the longer path
	currentCost = 12;

	//haven't gone anywhere yet
	distTravelled = 0;

	//populate list of all nodes
	allNodes.push_back(block1);
	allNodes.push_back(block2);
	allNodes.push_back(block3);
	allNodes.push_back(block4);
	allNodes.push_back(block5);
	allNodes.push_back(block6);
	allNodes.push_back(block7);
	allNodes.push_back(block8);
	allNodes.push_back(block9);
	allNodes.push_back(block10);
	allNodes.push_back(block11);
	allNodes.push_back(block12);
	allNodes.push_back(block13);
	allNodes.push_back(block14);
	allNodes.push_back(block15);
	allNodes.push_back(block16);
	allNodes.push_back(block17);
	allNodes.push_back(block18);
}

//overload == operator to allow node comparison
bool operator==(NODE &n1, const NODE &n2)
{
	return &n1 == &n2;
}

//use A* search algorithm to find best path
//returns true if going up is shorter
//returns false if going right is shorter
bool AStar::RunSearch()
{
	//while we're not yet at the goal, should start with block1
	while (openSet.size() < 10) {

		//reset currentCost
		currentCost = 12;

		//go to node in openSet with lowest cost, might need to set that manually
		for each (NODE curr in openSet)
		{
			if (curr.cost < currentCost) {

				//because currentCost is being decreased each loop, this will find the smallest curr.cost in openSet
				//making travelTo the least expensive node to use
				currentCost = curr.cost;
				travelTo = curr;
			}
		}
		
		//if current node is the goal and we used the shorter path, you're done
		if (travelTo.thisName == "goal" && distTravelled < 12) {
			upOrRight = true;
			return true;
		}

		//otherwise remove current node from openSet and add to closedSet
		else {
			//find travelTo in openSet, move it to the back, and then remove it
			auto pos = std::find(openSet.begin(), openSet.end(), travelTo) - openSet.begin();
			openSet.erase(openSet.begin() + pos - 1);
		}

		//for each of the four directions of travelTo, check if they exist.
		//in up-right-down-left order to give preference to up and right movements,
		//which bring us closer to the goal
		if (travelTo.up.exists) {

			//if the node is already in closedSet
			if (std::find(closedSet.begin(), closedSet.end(), travelTo.up.nodeName) != closedSet.end()) {
				continue; //ignore neighbor nodes already in closedSet
			}
			else { //if not in openSet, add neighbor to openSet
				//search for node with name the same as travelTo.up.nodeName
				for each (NODE search in allNodes)
				{
					if (search.thisName == travelTo.up.nodeName) {
						//that is the node to push to openset
						openSet.push_back(search);
					}
				}
			}
		}
		if (travelTo.right.exists) {

			//if the node is already in closedSet
			if (std::find(closedSet.begin(), closedSet.end(), travelTo.right.nodeName) != closedSet.end()) {
				continue; //ignore neighbor nodes already in closedSet
			}
			else { //if not in openSet, add neighbor to openSet
				   //search for node with name the same as travelTo.up.nodeName
				for each (NODE search in allNodes)
				{
					if (search.thisName == travelTo.right.nodeName) {
						//that is the node to push to openset
						openSet.push_back(search);
					}
				}
			}
		}
		if (travelTo.down.exists) {

			//if the node is already in closedSet
			if (std::find(closedSet.begin(), closedSet.end(), travelTo.down.nodeName) != closedSet.end()) {
				continue; //ignore neighbor nodes already in closedSet
			}
			else { //if not in openSet, add neighbor to openSet
				   //search for node with name the same as travelTo.up.nodeName
				for each (NODE search in allNodes)
				{
					if (search.thisName == travelTo.down.nodeName) {
						//that is the node to push to openset
						openSet.push_back(search);
					}
				}
			}
		}
		if (travelTo.left.exists) {

			//if the node is already in closedSet
			if (std::find(closedSet.begin(), closedSet.end(), travelTo.left.nodeName) != closedSet.end()) {
				continue; //ignore neighbor nodes already in closedSet
			}
			else { //if not in openSet, add neighbor to openSet
				   //search for node with name the same as travelTo.up.nodeName
				for each (NODE search in allNodes)
				{
					if (search.thisName == travelTo.left.nodeName) {
						//that is the node to push to openset
						openSet.push_back(search);
					}
				}
			}
		}

		//print node just visited
		cout << travelTo.thisName << "\n";
		
		//increment distTravelled because we moved forward 1 node
		distTravelled++;
	}

	//if current node is the goal and we used the shorter path, you're done
	if (travelTo.right.nodeName == "goal" && distTravelled < 12) {
		upOrRight = true;
		cout << "Print 1 if up is shorter path: " << upOrRight << "\n\n";
		return true;
	}

	//going right is shorter (which is wrong), just a default value
	return false;
}