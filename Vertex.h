#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition
// Contains just a position and color for now
// --------------------------------------------------------
struct Vertex
{
	DirectX::XMFLOAT3 Position;	    // The local position of the vertex 
	DirectX::XMFLOAT4 Color;        // The color of the vertex
};

