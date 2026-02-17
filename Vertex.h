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

inline DirectX::XMFLOAT4 Red(1.0f, 0.0f, 0.0f, 1.0f);
inline DirectX::XMFLOAT4 Blue(0.0f, 0.0f, 1.0f, 1.0f);
inline DirectX::XMFLOAT4 Green(0.0f, 1.0f, 0.0f, 1.0f);
inline DirectX::XMFLOAT4 White(1.0f, 1.0f, 1.0f, 1.0f);
inline DirectX::XMFLOAT4 Black(0.0f, 0.0f, 0.0f, 0.0f);
