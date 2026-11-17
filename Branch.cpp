#include "Branch.h"
#include <cmath>

Branch::Branch(BorderedBox* rb, float x, float y, float d, float endRelX, float endRelY, float endD)
{
	renderBox = rb;
	wholeOffset = DirectX::XMFLOAT2(x, y);
	nodesDiameter[0] = d; // 
	nodeCount = 1; // The node count is not 0-indexed -> it is a simple count!
	AddNode(endRelX, endRelY, endD);
	enabled = false; 
	toGrow = true;
	distCovered = 0; // We start at no distance covered.
}

Branch::~Branch() 
{
}

void Branch::AddNode(float relX, float relY, float diameter)
{
	// Note when entering the method, we haven't incremented the node count. 
	// This is very handy! 
	// For example, when creating a branch, we create the first node in the constructor, and nodeCount is 1
	// Now we want to store data (relative position and diameter) pertaining to the second node (index 1). 
	// Use node count first, before incrementing it. 

	if (nodeCount == maxNodes) // can't have more than a certain number of nodes in the branch
	{
		return;
	}
	
	nodesRelativeXY[nodeCount] = DirectX::XMFLOAT2(relX, relY); 
	nodesDiameter[nodeCount] = diameter; 
	nodeCount++;
}

void Branch::PopNode() 
{
	if (nodeCount == 2) { return; }

	nodesRelativeXY[nodeCount].x = -1;
	nodesRelativeXY[nodeCount].y = -1;
	nodesDiameter[nodeCount] = 0;
	if (desiredEnd == nodeCount) { CalculateSummedDistance(nodeCount - 1); }
	nodeCount--;
	
	
}

/// <summary>
/// // Used to calculate distance from the start node to the current end node.
/// </summary>
void Branch::CalculateSummedDistance(int desiredEndCount)
{
	// Less than 2 nodes is an invalid setup
	if (desiredEndCount < 2) { return; }
	// If we are trying to calculate distance for nodes that do not exist yet, it is invalid 
	if (desiredEndCount > nodeCount) { return; }

	summedDist = 0;
	int counter = 1;
	desiredEnd = desiredEndCount;
	while (counter < desiredEndCount)
	{
		summedDist += relativeDistances[counter - 1];
		counter++;
	}
	// To complete the branch, the last node also has to grow, so add its diameter!
	summedDist += nodesDiameter[desiredEnd - 1];
}

void Branch::CalculateRDAngles() 
{
	nodesRelativeXY[0] = DirectX::XMFLOAT2(0,0);
	int counter = 1; // I start my counter from the second node's index now
	DirectX::XMFLOAT2 xy; // temporary set of coordinates to jump from one node to the next

	while (counter < nodeCount) 
	{
		xy = nodesRelativeXY[counter]; 
		relativeDistances[counter - 1] = std::sqrtf(std::powf(xy.x, 2) + std::powf(xy.y, 2));
		angles[counter - 1] = atan2(xy.y, xy.x);
		counter++;
	}
}

void Branch::ChangeDistance(float deltaTime) 
{
	if (enabled) // Only change if the branch can be displayed
	{
		distCovered += toGrow ? deltaTime : deltaTime * -1;
	}
	
}




void Branch::Draw()
{
	if (!enabled) { return; }

	// Clamp the distance first
	distCovered = distCovered > summedDist ? summedDist : distCovered;
	distCovered = distCovered < 0 ? 0 : distCovered;

	if (distCovered == 0) { enabled = false; toGrow = true; return; }
	if (distCovered == summedDist)
	{
		DrawAll();
		return;
	}
	
	float distRemaining = distCovered;

	// populate the array with -1 
	// handy for knowing when to stop drawing circles!
	for (int i = 0; i < maxNodes; i++)
	{
		nodesPartialDiameters[i] = -1;
	} 
	
	nodesToDraw = 0;
	currentLineDist = 0;

	// -- RENDER LINES FIRST ( as they go behind the circles )--
	renderBox->SetKind(SmoothLine);
	renderBox->SetInvSmoothIntensity(1);
	renderBox->SetOpacity(1);

	counter = 0;
	drawXOffset = wholeOffset.x;
	drawYOffset = wholeOffset.y;
	
	// The loop should run until we either run out of distance or run out of lines to draw
	while (distRemaining > 0 && counter < desiredEnd-1)
	{
		// Get to the right position
		// Since relative positions were used, simply keep adding!
		drawXOffset += nodesRelativeXY[counter].x;
		drawYOffset += nodesRelativeXY[counter].y;

		// Size the line appropriately
		// Either the line is definitely covered or it is partially covered
		if (distRemaining > relativeDistances[counter]) 
		{
			currentLineDist = relativeDistances[counter];
		}
		else 
		{
			currentLineDist = distRemaining;
		}
		distRemaining -= currentLineDist;

		// Depending on the length of the line, we can also determine how big or small the circle should be
		// The length of the line simply needs to be bigger than the length of the diameter of the circle to draw it entirely, 
		// otherwise, size it to the line!
		nodesPartialDiameters[counter] = currentLineDist > nodesDiameter[counter] ? nodesDiameter[counter] : currentLineDist;
		nodesToDraw++;

		// Draw 2 lines to give the illusion of a bordered line
		renderBox->SetXY(drawXOffset, drawYOffset); 
		renderBox->SetAngle(angles[counter]);

		renderBox->SetWidthHeight(currentLineDist, lineThickness + 2);
		renderBox->SetColor(1, 0, 0);
		renderBox->Draw(0);

		renderBox->SetWidthHeight(currentLineDist, lineThickness);
		renderBox->SetColor(1, 1, 1);
		renderBox->Draw(0);

		counter++;
	}

	if (distRemaining > 0) // Meaning we went through all the lines and the last node can be drawn
	{
		nodesPartialDiameters[desiredEnd - 1] = distRemaining;
	}

	// -- RENDER CIRCLES NEXT --
	renderBox->SetKind(Circle);
	renderBox->SetOpacity(1);
	renderBox->SetAngle(0);
	
	// Start from the first node again!
	// Drawing on top of the lines. 
	counter = 0;
	drawXOffset = wholeOffset.x;
	drawYOffset = wholeOffset.y;

	while (nodesPartialDiameters[counter] != -1)
	{
		drawXOffset += nodesRelativeXY[counter].x;
		drawYOffset += nodesRelativeXY[counter].y;
		drawDiameter = nodesPartialDiameters[counter];
		drawRadius = drawDiameter / 2;
		renderBox->SetWidthHeight(drawDiameter, drawDiameter);
		renderBox->SetXY(drawXOffset - drawRadius, drawYOffset - drawRadius);
		renderBox->Draw(0);
		counter++;
	}
}

void Branch::DrawAll()
{
	
	drawXOffset = wholeOffset.x;
	drawYOffset = wholeOffset.y;
	counter = 0;

	// -- RENDER LINES FIRST --
	renderBox->SetKind(SmoothLine);
	renderBox->SetInvSmoothIntensity(1);
	renderBox->SetOpacity(1);
	counter = 0;
	drawXOffset = wholeOffset.x;
	drawYOffset = wholeOffset.y;
	while (counter < desiredEnd - 1)
	{
		drawXOffset += nodesRelativeXY[counter].x;
		drawYOffset += nodesRelativeXY[counter].y;

		renderBox->SetWidthHeight(relativeDistances[counter], lineThickness + 2);
		renderBox->SetColor(1, 0, 0);
		renderBox->SetXY(drawXOffset, drawYOffset);
		renderBox->SetAngle(angles[counter]);
		renderBox->Draw(0);

		renderBox->SetWidthHeight(relativeDistances[counter], lineThickness);
		renderBox->SetColor(1, 1, 1);
		
		
		renderBox->Draw(0);
		counter++;
	}

	// -- RENDER CIRCLES NEXT --
	renderBox->SetKind(Circle);
	renderBox->SetOpacity(1);
	renderBox->SetAngle(0);
	float drawDiameter, drawRadius;
	counter = 0;
	drawXOffset = wholeOffset.x;
	drawYOffset = wholeOffset.y;
	while (counter < desiredEnd)
	{
		drawXOffset += nodesRelativeXY[counter].x;
		drawYOffset += nodesRelativeXY[counter].y;
		drawDiameter = nodesDiameter[counter];
		drawRadius = drawDiameter / 2;
		renderBox->SetWidthHeight(drawDiameter, drawDiameter);
		renderBox->SetXY(drawXOffset - drawRadius, drawYOffset - drawRadius);
		renderBox->Draw(0);
		counter++;
	}
}