#pragma once
#include "RectangleEntity.h"
#include <vector>
#include "Material.h"

class RoundedRectangleEntity
{
// Would need three rectangle entities with four circles - Seven Rectangle Entities in total
// Could implement semi-circle shader as well
// Three have RectVS+PixelShader, four have RectVS+CirclePS
// Render the circles first, then the above rectangles

// Would be nice to find a graph, and then simply have two rectangle entities reflected vertically

// If radius is 0 - render a single rectangle, the one who has the center transform
// Once radius is larger than 1, subtract radius from width, to get the two bordering rectangles, whose height will also be reduced by the radius both from the top and the bottom
// Use radius to determine all that needs to be rendered

public:
	RoundedRectangleEntity(std::shared_ptr<Material> r, std::shared_ptr<Material> c, DirectX::XMFLOAT2 wh = DirectX::XMFLOAT2(3, 3), float radius = 1, DirectX::XMFLOAT2 xy = DirectX::XMFLOAT2(200, 200));
	void CalculateSetDimensions();
	
	void SetRadius(float radius);
	void SetWidthHeight(float w, float h);
	void SetWidthHeight(DirectX::XMFLOAT2 wh);

	void Draw(unsigned int screenWidth, unsigned int screenHeight);

	DirectX::XMFLOAT2 GetWidthHeight();
	float GetRadius();

private:
	std::vector<RectangleEntity> parts;
	std::shared_ptr<Material> rectMaterial, circleMaterial;
	float radius;
	float width, height;
	int edited;
	DirectX::XMFLOAT2 translateXY;
	DirectX::XMFLOAT4 color;
};

