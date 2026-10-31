#include "Branch.h"
#include <cmath>

Branch::Branch(float x, float y, float startRadius, float endRelX, float endRelY)
{
	wholeOffset.x = x;
	wholeOffset.y = y;

	int factor = startRadius < endNodeRadius ? 1 : -1;


	nodesRelativeXY[0] = DirectX::XMFLOAT2(0, 0);
	nodeCount = 1;
	AddNode(endRelX, endRelY);
	

	distCovered = 0;
}

Branch::~Branch() 
{
	delete renderBox;
}

void Branch::AddNode(float relX, float relY)
{
	if (nodeCount > 9) 
	{
		return;
	}

	nodesRelativeXY[nodeCount].x = relX;
	nodesRelativeXY[nodeCount].y = relY;
	CalculateDistance(++nodeCount);
	sizeDifference = abs(startRadius - endNodeRadius) / nodeCount;
}

float Branch::CalculateDistance(int index)
{
	if (index == 0) { return 0; }
	if (index > maxNodes - 1) { return -1; }
	int counter = 1;
	DirectX::XMFLOAT2 xy;
	while (counter < index) 
	{
		xy = nodesRelativeXY[counter];
		relativeDistances[counter-1] = std::sqrtf(std::powf(xy.x, 2) + std::powf(xy.y, 2));
		angles[counter - 1] = atan2(xy.y, xy.x);
		counter++;
	}
}

void Branch::Draw(float deltaTime, BorderedBox* renderBox)
{
	// -- RENDER LINES FIRST --
	renderBox->SetKind(SmoothLine);
	renderBox->SetOpacity(1);
	int counter = 0;
	float xOffset = wholeOffset.x;
	float yOffset = wholeOffset.y;
	while (counter < nodeCount - 1) 
	{
		xOffset += nodesRelativeXY[counter].x;
		yOffset += nodesRelativeXY[counter].y;
		renderBox->SetWidthHeight(relativeDistances[counter], 5);
		renderBox->SetXY(xOffset, yOffset);
		renderBox->SetColor(1, 0, 0);
		renderBox->SetAngle(angles[counter]);
		renderBox->Draw(deltaTime);
		counter++;
	}
	
	
	
	
	
}