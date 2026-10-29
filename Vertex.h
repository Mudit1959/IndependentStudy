#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A vertex defines a point in space. 
// It can have additional data attached to it, such as color, surface normal, texture information. 
// Essentially, a chunk of memory with all the data necessary to render a specific point in space. 
// --------------------------------------------------------
struct Vertex
{
	DirectX::XMFLOAT3 Position;	    // The local position of the vertex
	DirectX::XMFLOAT2 UV;			// The X and Y UV coordinates -> goes from 0 to 1 (left to right & up to down)
};

struct BoxData 
{
	DirectX::XMFLOAT2 offset;
	DirectX::XMFLOAT2 screenSize;

	DirectX::XMFLOAT2 boxSize;
	float opacity;
	float angle;

	DirectX::XMFLOAT3 color; // -> from 0 to 255
	float inverseSmoothIntensity; // 0 to 20 -> closer to 20, the sharper the to and bottom edges of the box are.
};