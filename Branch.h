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
	void CalculateDistance(int index);
	void Draw(float deltaTime, BorderedBox* renderBox);
	~Branch();

private:
	BorderedBox* renderBox; // I want a pointer to one box object that I use repeatedly during each draw call

	// I want a way to track all the X and Y positions of each node, with a maximum of 10 nodes
	// The start node can be of any size, the end node must be of size 10 -> could change this.
	

	float relativeDistances[maxNodes - 1]; // The lines in between the nodes are always 1 less in number than the total node count
	float angles[maxNodes - 1];
	
	DirectX::XMFLOAT2 nodesRelativeXY[maxNodes];
	float nodesDiameter[maxNodes];
	DirectX::XMFLOAT2 wholeOffset; 

	int nodeCount; // Keep track of the nodes made
	float distCovered; 
	float summedDist;

};

