#include "Branch.h"
#include <cmath>

Branch::Branch(float x, float y, float d, float endRelX, float endRelY, float endD)
{
	wholeOffset = DirectX::XMFLOAT2(x, y);
	nodesDiameter[0] = d;
	nodeCount = 1;
	AddNode(endRelX, endRelY, endD);
	enabled = false;
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
	summedDist = 0;
	while (counter < nodes) 
	{
		xy = nodesRelativeXY[counter];
		relativeDistances[counter-1] = std::sqrtf(std::powf(xy.x, 2) + std::powf(xy.y, 2));
		summedDist += relativeDistances[counter - 1];
		angles[counter - 1] = atan2(xy.y, xy.x);
		counter++;
	}
}

void Branch::ChangeDistance(float deltaTime) 
{
	distCovered += enabled ? deltaTime : deltaTime * -1;
}

void Branch::ClampDistance() 
{
	float totalDist = summedDist + nodesDiameter[nodeCount - 1];
	distCovered = distCovered > totalDist ? totalDist : distCovered;
	distCovered = distCovered < 0 ? 0 : distCovered;
}

void Branch::SetDistCovered(float d) { distCovered = d; }

void Branch::Draw(BorderedBox* renderBox)
{
	ClampDistance();

	if (!enabled && distCovered == 0) { return; }

	if (distCovered == summedDist + nodesDiameter[nodeCount - 1])
	{
		DrawAll(renderBox);
		return;
	}

	int counter;
	float xOffset, yOffset;
	float distRemaining = distCovered;
	float currentLineDist;

	float nodesPartialDiameters[maxNodes] = {-1};
	int nodesToDraw = 0;

	// -- RENDER LINES FIRST --
	renderBox->SetKind(SmoothLine);
	renderBox->SetInvSmoothIntensity(1);
	renderBox->SetOpacity(1);
	counter = 0;
	xOffset = wholeOffset.x;
	yOffset = wholeOffset.y;
	while (distRemaining > 0 && counter < nodeCount-1)
	{
		xOffset += nodesRelativeXY[counter].x;
		yOffset += nodesRelativeXY[counter].y;

		if (distRemaining > relativeDistances[counter]) 
		{
			currentLineDist = relativeDistances[counter];
		}
		else 
		{
			currentLineDist = distRemaining;
		}

		distRemaining -= currentLineDist;
		nodesPartialDiameters[counter] = currentLineDist > nodesDiameter[counter] ? nodesDiameter[counter] : currentLineDist;
		nodesToDraw++;
		renderBox->SetWidthHeight(currentLineDist, lineThickness + 2);
		renderBox->SetColor(1, 0, 0);
		renderBox->SetXY(xOffset, yOffset);
		renderBox->SetAngle(angles[counter]);
		renderBox->Draw(0);

		renderBox->SetWidthHeight(currentLineDist, lineThickness);
		renderBox->SetColor(1, 1, 1);
		renderBox->Draw(0);

		counter++;
	}

	if (counter == nodeCount - 1 && distRemaining > 0) 
	{
		nodesPartialDiameters[nodeCount - 1] = distRemaining;
	}

	// -- RENDER CIRCLES NEXT --
	renderBox->SetKind(Circle);
	renderBox->SetOpacity(1);
	renderBox->SetAngle(0);
	float drawDiameter, drawRadius, currentDrawDiameter;
	counter = 0;
	xOffset = wholeOffset.x;
	yOffset = wholeOffset.y;
	while (counter < nodeCount && nodesPartialDiameters[counter] != -1)
	{
		xOffset += nodesRelativeXY[counter].x;
		yOffset += nodesRelativeXY[counter].y;
		drawDiameter = nodesPartialDiameters[counter];
		drawRadius = drawDiameter / 2;
		renderBox->SetWidthHeight(drawDiameter, drawDiameter);
		renderBox->SetXY(xOffset - drawRadius, yOffset - drawRadius);
		renderBox->Draw(0);
		counter++;
	}
}

void Branch::DrawAll(BorderedBox* renderBox)
{
	int counter;
	float xOffset, yOffset;

	// -- RENDER LINES FIRST --
	renderBox->SetKind(SmoothLine);
	renderBox->SetInvSmoothIntensity(1);
	renderBox->SetOpacity(1);
	counter = 0;
	xOffset = wholeOffset.x;
	yOffset = wholeOffset.y;
	while (counter < nodeCount - 1)
	{
		xOffset += nodesRelativeXY[counter].x;
		yOffset += nodesRelativeXY[counter].y;

		renderBox->SetWidthHeight(relativeDistances[counter], lineThickness + 2);
		renderBox->SetColor(1, 0, 0);
		renderBox->SetXY(xOffset, yOffset);
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
		renderBox->Draw(0);
		counter++;
	}
}