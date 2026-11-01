#include "Branch.h"
#include <cmath>

Branch::Branch(float x, float y, float d, float endRelX, float endRelY, float endD)
{
	wholeOffset = DirectX::XMFLOAT2(x, y);
	nodesDiameter[0] = d;
	nodeCount = 1;
	AddNode(endRelX, endRelY, endD);
	

	distCovered = 0;
}

Branch::~Branch() 
{
}

void Branch::AddNode(float relX, float relY, float diameter)
{
	if (nodeCount == 10) 
	{
		return;
	}

	nodesRelativeXY[nodeCount] = DirectX::XMFLOAT2(relX, relY);
	nodesDiameter[nodeCount] = diameter;
	nodeCount++;
	CalculateDistance(nodeCount);
}

void Branch::PopNode() 
{
	if (nodeCount == 2) { return; }
	nodesRelativeXY[nodeCount].x = -1;
	nodesRelativeXY[nodeCount].y = -1;
	nodesDiameter[nodeCount] = 0;
	nodeCount--;
	CalculateDistance(nodeCount);
}

void Branch::CalculateDistance(int nodes)
{
	if (nodes < 2) { return; }
	if (nodes > maxNodes) { return; }
	int counter = 1;
	DirectX::XMFLOAT2 xy;
	while (counter < nodes) 
	{
		xy = nodesRelativeXY[counter];
		relativeDistances[counter-1] = std::sqrtf(std::powf(xy.x, 2) + std::powf(xy.y, 2));
		angles[counter - 1] = atan2(xy.y, xy.x);
		counter++;
	}
}

void Branch::Draw(float deltaTime, BorderedBox* renderBox)
{
	int counter;
	float xOffset, yOffset;

	// -- RENDER LINES FIRST --
	renderBox->SetKind(SmoothLine);
	renderBox->SetInvSmoothIntensity(7);
	renderBox->SetOpacity(1);
	counter = 0;
	xOffset = wholeOffset.x;
	yOffset = wholeOffset.y;
	while (counter < nodeCount - 1)
	{
		xOffset += nodesRelativeXY[counter].x;
		yOffset += nodesRelativeXY[counter].y;
		renderBox->SetWidthHeight(relativeDistances[counter], 5);
		renderBox->SetXY(xOffset, yOffset);
		renderBox->SetColor(1, 1, 1);
		renderBox->SetAngle(angles[counter]);
		renderBox->Draw(deltaTime);
		counter++;
	}

	// -- RENDER CIRCLES NEXT --
	renderBox->SetKind(Circle);
	renderBox->SetOpacity(1);
	renderBox->SetAngle(0);
	float drawDiameter, drawRadius;
	counter = 0;
	xOffset = wholeOffset.x;
	yOffset = wholeOffset.y;
	while (counter < nodeCount)
	{
		xOffset += nodesRelativeXY[counter].x;
		yOffset += nodesRelativeXY[counter].y;
		drawDiameter = nodesDiameter[counter];
		drawRadius = drawDiameter / 2;
		renderBox->SetWidthHeight(drawDiameter, drawDiameter);
		renderBox->SetXY(xOffset - drawRadius, yOffset - drawRadius);
		renderBox->Draw(deltaTime);
		counter++;
	}

	renderBox->SetWidthHeight(nodesDiameter[0], nodesDiameter[0]);
	renderBox->SetXY(wholeOffset.x + 30, wholeOffset.y);
	renderBox->Draw(deltaTime);
	
	
}