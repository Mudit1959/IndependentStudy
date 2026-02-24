#include "RoundedRectangleEntity.h"

/// <summary>
/// Creates a rounded rectangle, by default 3px tall, 3px wide, with 1px radius on each corner
/// </summary>
/// <param name="r">The Material(VS and PS) to render a rectangle</param>
/// <param name="c">The Material(VS and PS) to render a circle</param>
/// <param name="xy">The pixel coordinates of the center on screen - Top left is (0,0) | Right and down the screen are positive</param>
/// <param name="wh">The pixel width and height of the rounded rect, ignoring the rounded corners</param>
/// <param name="inRadius">The radius of the rounded corners</param>
RoundedRectangleEntity::RoundedRectangleEntity(std::shared_ptr<Material> r, std::shared_ptr<Material> c, DirectX::XMFLOAT2 wh, float inRadius, DirectX::XMFLOAT2 xy) :
	rectMaterial(r), circleMaterial(c), translateXY(xy)
{
	edited = 0;

	// The two materials required to render have been defined, so has the center of the rounded rectangle
	parts.push_back(RectangleEntity(rectMaterial, RECT)); // The center rectangle
	parts.push_back(RectangleEntity(rectMaterial, RECT)); // Left Rect
	parts.push_back(RectangleEntity(rectMaterial, RECT)); // Right Rect

	// The four circles defining rounded corners going clockwise
	parts.push_back(RectangleEntity(circleMaterial, CIRCLE));
	parts.push_back(RectangleEntity(circleMaterial, CIRCLE));
	parts.push_back(RectangleEntity(circleMaterial, CIRCLE));
	parts.push_back(RectangleEntity(circleMaterial, CIRCLE));
	
	// Width, Height and Radius are stored - The object has been "edited"
	width = wh.x;
	height = wh.y;
	radius = inRadius;
	edited++; 

	color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	for(int i =0; i<7; i++)
	{
		parts[i].SetColor(color);
	}

	CalculateSetDimensions();
}

/// <summary>
/// Checks if valid parameters have been given, else generates a sharp-edged rectangle
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="radius"></param>
void RoundedRectangleEntity::CalculateSetDimensions() 
{
	if (edited == 0) { return; }

	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;

	if (radius > height/2.0f || radius > width/2.0f) { radius = 0; }

	float widthCenter = width - (radius * 2.0f);
	float halfWidthCenter = widthCenter / 2.0f;

	float heightSides = height - (radius * 2.0f);
	float halfHeightSides = heightSides / 2.0f;

	float halfRadius = (radius / 2.0f);

	//Center Rect
	parts[0].GetTransform()->SetScale(halfWidthCenter, halfHeight, 0.0f);
	parts[0].SetTranslateXY(translateXY);

	//Left Rect
	parts[1].GetTransform()->SetScale(halfRadius, halfHeightSides, 0.0f);
	parts[1].SetTranslateXY(translateXY.x - halfWidthCenter - halfRadius, translateXY.y);

	//Right Rect
	parts[2].GetTransform()->SetScale(halfRadius, halfHeightSides, 0.0f);
	parts[2].SetTranslateXY(translateXY.x + halfWidthCenter + halfRadius, translateXY.y);

	//Top Left
	parts[3].GetTransform()->SetScale(radius, radius, 0.0f);
	parts[3].SetTranslateXY(translateXY.x - halfWidthCenter, translateXY.y - halfHeightSides);

	//Top Right
	parts[4].GetTransform()->SetScale(radius, radius, 0.0f);
	parts[4].SetTranslateXY(translateXY.x + halfWidthCenter, translateXY.y - halfHeightSides);

	//Bottom Right
	parts[5].GetTransform()->SetScale(radius, radius, 0.0f);
	parts[5].SetTranslateXY(translateXY.x + halfWidthCenter, translateXY.y + halfHeightSides);

	//Bottom Left
	parts[6].GetTransform()->SetScale(radius, radius, 0.0f);
	parts[6].SetTranslateXY(translateXY.x - halfWidthCenter, translateXY.y + halfHeightSides);

	edited = 0;
}

void RoundedRectangleEntity::Draw(unsigned int screenWidth, unsigned int screenHeight) 
{
	if (edited != 0) { CalculateSetDimensions(); }
	
	// If the radius is 0, we only want to draw the center rectangle
	if (radius == 0) 
	{
		parts[0].DrawRect(screenWidth, screenHeight);
	}
	// If the radius is not 0, we want to draw the rectangles first, then the rounded corners 
	// - This is for future designs, like having sharp edges on the inside and colored rounded corners
	// The pixel shader adds colors?
	else
	{
		parts[0].DrawRect(screenWidth, screenHeight);
		parts[1].DrawRect(screenWidth, screenHeight);
		parts[2].DrawRect(screenWidth, screenHeight);

		parts[3].DrawCircle(screenWidth, screenHeight, radius);
		parts[4].DrawCircle(screenWidth, screenHeight, radius);
		parts[5].DrawCircle(screenWidth, screenHeight, radius);
		parts[6].DrawCircle(screenWidth, screenHeight, radius);
		
	}
}

void RoundedRectangleEntity::SetRadius(float r) { radius = r; edited++; }
void RoundedRectangleEntity::SetWidthHeight(float w, float h) { width = w; height = h; edited++; }
void RoundedRectangleEntity::SetWidthHeight(DirectX::XMFLOAT2 wh) { width = wh.x; height = wh.y; }

DirectX::XMFLOAT2 RoundedRectangleEntity::GetWidthHeight() { return DirectX::XMFLOAT2(width, height); }
float RoundedRectangleEntity::GetRadius() {return radius;}


