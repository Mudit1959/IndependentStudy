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
	DirectX::XMFLOAT4 Color;        // The color of the vertex
};

struct BoxData 
{
	DirectX::XMFLOAT2 offset;
	DirectX::XMFLOAT2 screenSize;

	DirectX::XMFLOAT2 boxSize;
	float opacity;
	float angle;

	DirectX::XMFLOAT4X4 worldMatrix;

};