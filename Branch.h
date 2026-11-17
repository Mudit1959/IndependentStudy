#pragma once

#include "BorderedBox.h"

static const int maxNodes = 10;

class Branch
{

public:
	Branch(float x, float y, float diameter, float endRelX = 20, float endRelY = -20, float endD = 20);
	void AddNode(float relX, float relY, float diameter);
	void AddDistance(float deltaTime);
	void RemoveDistance(float deltaTime);
	void PopNode();
	void CalculateSummedDistance(int nodes); 
	void CalculateMaximumLength();
	void CalculateRDAngles();
	void DrawAll(BorderedBox* renderBox);
	void Draw(BorderedBox* renderBox);
	void ChangeDistance(float deltaTime);

	inline void SetDistCovered(float d) { distCovered = d > summedDist ? summedDist : d; }
	
	inline void ToggleEnable(bool e) { enabled = e; }
	inline void ToggleIncrDecr(bool e) { toGrow = e; }
	~Branch();

private:

	// A branch consists of several circle/nodes, linked by lines, that grow and shrink. 
	// When growing, the line extends to the center of a node. When it arrives, the line continues to grow, but it also prompts the node to grow to its full diameter.

	BorderedBox* renderBox; // I want a pointer to one box object that I use repeatedly during each draw call

	// I want a way to track all the X and Y positions of each node, with a maximum of ... nodes
	// The start node can be of any size, the end node must be of size 10 -> could change this.
	
	bool enabled; // tracks whether the branch should be visible or not
	bool toGrow; // tracks whether the branch should grow or not

	// All the arrays are created to accomodate the maximum number of nodes, regardless of whether we have those many nodes or not

	float nodesDiameter[maxNodes]; // Diameters of each node
	// Every node treats the previous as the origin, and is positioned relative to it
	// I never touch the values in index 0, since my first node is positioned at the origin (0,0)
	DirectX::XMFLOAT2 nodesRelativeXY[maxNodes]; 
	float relativeDistances[maxNodes - 1]; // The lines in between the nodes are always 1 less in number than the total node count
	float angles[maxNodes - 1]; // The angle of each line
	
	
	
	DirectX::XMFLOAT2 wholeOffset; // The position of the branch as a whole
	const float lineThickness = 3; // Thickness of the line

	int nodeCount; // How many nodes have been made?
	int desiredEnd; // Where do we want the branch to end?
	float distCovered; // This tracks the distance as we increment or decrement it
	float summedDist; // This tracks the maximum possible length the branch can extend to

	// For drawing a partially completed line
	int counter; // for looping
	float nodesPartialDiameters[maxNodes]; 
	float drawXOffset, drawYOffset;
	float currentLineDist; // to size the line at each node
	int nodesToDraw; // how many circles should I draw?
	float drawDiameter, drawRadius; // for the circles
	

};

